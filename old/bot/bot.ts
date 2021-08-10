import { exit } from "process";
import { net,scrapeToken } from "./discord/discord"
import { parser } from "./parse/parser"
import * as WebSocket from "ws"

let botConfig: string[] = parser.confToJson("bot/bot.conf");
let TOKEN: string;
let userData = {};
let client = {};

async function waitfor(time:number) {
  return new Promise<void>(res => {
    setTimeout(res,time);
  });
}

async function login() {
  if (botConfig["SCRAPETOKEN"] == true) {
    let s:string = scrapeToken();
    if (s == null){
      console.log("Token couldn't be scraped.");
    }
  } if (botConfig["LOGINUSER"] != null && botConfig["LOGINPASSWORD"] != null && !TOKEN) {
    let res = await net.sendApiReq("auth/login", Buffer.from(JSON.stringify({
      login: botConfig["LOGINUSER"],
      password: botConfig["LOGINPASSWORD"],
      undelete: false,
      captcha_key: null,
      login_source: null,
      gift_code_sku_id: null
    })),"POST");

    let data = JSON.parse(res.content.toString());
    TOKEN = data["token"];
  } else if (botConfig["TOKEN"] != null) {
    let res = await net.sendApiReq("users/@me",null,null,botConfig["TOKEN"]);
    if (res.status.code != 401) {
      TOKEN = botConfig["TOKEN"];
    }
  } else {
    console.log("No token or method to get token was given or worked. Edit bot/bot.conf");
    exit(1);
  }
}

let wsc = 0;
async function bot() {
  let sock = new WebSocket("wss://gateway.discord.gg/?encoding=json&v=9");
  sock.on("message",(m) => {
    m = JSON.parse(m.toString());
    if (m["t"] == null)
      switch (m["op"]) {
        case 11:
          wsc--;
          break;
        case 10:
          net.heartbeatws(sock,m["d"]["heartbeat_interval"]);
          wsc--;
          sock.send(JSON.stringify({
            op:2,
            d:{
              token: TOKEN,
              capabilities:125,
              properties:{
                os:"linux",
                browser:"Firefox",
                device:"",
                system_locale:"en-US",
                browser_user_agent:"Mozilla/5.0 (Linux) Gecko/20100101 Firefox/89.0",
                browser_version:"89.0",
                os_version:"",
                referrer:"",
                referring_domain:"",
                referrer_current:"",
                release_channel:"stable",
                client_build_number:89709,
                client_event_source:null
              },
              presence:{
                status:"online",
                since:0,
                activities:[],
                afk:false,
              },
              compress:false,
              client_state:{
                guild_hashes:{},
                highest_last_message_id:"0",
                read_state_version:0,
                user_guild_settings_version:-1
              }
            }
          }))
          break;
        default:
          console.log(m);
          break;
      }
    else {
      let data = m['d'];
      switch (m["t"]) {
        case "READY":
          client = data;
          console.log(`Client ready (${client['user']['username']}#${client['user']['discriminator']}).`);
          break;
        case "MESSAGE_CREATE":
          if (data["author"]["id"] == client["user"]["id"])
          cmd(data["content"],data);
          break;
        case "READY_SUPPLEMENTAL":
        case "PRESENCE_UPDATE":
        case "SESSIONS_REPLACE":
        case "MESSAGE_ACK":
        case "MESSAGE_DELETE":
        case "MESSAGE_DELETE_BULK":
        case "GUILD_MEMBER_UPDATE":
        case "GUILD_MEMBER_REMOVE":
        case "GUILD_BAN_ADD":
          break;
        default:
          console.log(m);
      }
    }
    wsc++;
    net.updateheartbeatc(wsc);
  });

  await new Promise(resolve => sock.once('open', resolve));
}

async function error(err,data) {
  await reply(`> Error (${err['code']}): ${err['message']}`,data);
}

async function reply(message:string,data) {
  let msg = await net.sendApiReq(`channels/${data["channel_id"]}/messages`, Buffer.from(JSON.stringify({content:message, tts:false})),"POST",TOKEN);
  let msgdata = JSON.parse(msg.content.toString());
  if (msgdata["id"] == undefined) {
    await error(msgdata,data);
    return false;
  }
  return true;
}

async function cmd(content:string,data) {
  if (content.startsWith(";")) {
    let cmd:string = content.slice(1,content.length);
    
    let args:string[] = cmd.split(/ /);

    let netr:{status:{code:number,message:string},content:Buffer};
    let reason:string = "No reason given";

    switch (args[0].normalize().toLowerCase()) {
      case "kick":
        if (!args[1]) {
          await reply("> You need to provide a member id.",data);
          break;
        }
        if (args.length > 2) {
          reason = args.slice(2,args.length).join(" ");
        }
        console.log(`guilds/${data['guild_id']}/members/${args[1]}?reason=${reason}`);
        netr = await net.sendApiReq(`guilds/${data['guild_id']}/members/${args[1]}?reason=${reason}`,null, "DELETE", TOKEN);

        if (netr.status.code != 204) {
          await error(JSON.parse(netr.content.toString()),data);
        }
        break;

      case "ban":
        if (!args[1]) {
          await reply("> You need to provide a member id.", data);
          break;
        }
        if (args.length > 2) {
          reason = args.slice(2,args.length).join(" ");
        }
        netr = await net.sendApiReq(`guilds/${data['guild_id']}/bans/${args[1]}`, Buffer.from(JSON.stringify({delete_message_days:0,reason:reason})), "PUT",TOKEN);

        if (netr.status.code != 204) {
          await error(JSON.parse(netr.content.toString()), data);
        }
        break;

        // do it manually, for some reason this shit dont work.
      // case "unban":
      //   if (!args[1]) {
      //     await reply("> You need to provide a member id.", data);
      //     break;
      //   }
      //   console.log(`guilds/${data['guild_id']}/bans/${args[1]}`);
      //   netr = await net.sendApiReq(`guilds/${data['guild_id']}/bans/${args[1]}`, null, "DELETE", TOKEN);

      //   console.log(netr.status, netr.content.toString());

      //   if (netr.status.code != 204) {
      //     await error(JSON.parse(netr.content.toString()), data);
      //   }
      //   break;

      case "purge":
        console.log(client["guilds"])

        netr = await net.sendApiReq(`channels/${data['channel_id']}/messages?limit=100`,null,"GET",TOKEN);
        let messages:string[] = JSON.parse(netr.content.toString());
        
        let first = true;
        while (messages.length >= 99 || first) {
          first = false;

          for (let i = 0, msg = messages[0]; i < messages.length; i++, msg = messages[i]) {
            await waitfor(150);
            let dm = await net.sendApiReq(`channels/${data['channel_id']}/messages/${msg['id']}`, null, "DELETE", TOKEN);
            if (dm.status.code == 429 && dm.content) {
              await waitfor(JSON.parse(dm.content.toString())["retry_after"]);
            }
          }

          if (messages.length >= 99) {
            netr = await net.sendApiReq(`channels/${data['channel_id']}/messages?limit=100`,null,"GET",TOKEN);
            messages = JSON.parse(netr.content.toString());
          }
        }
      break;
    }

    if (args[0].normalize().toLowerCase() != "purge")
      netr = await net.sendApiReq(`channels/${data['channel_id']}/messages/${data['id']}`, null, "DELETE", TOKEN);

    if (netr.status.code != 204) {
      error(JSON.parse(netr.content.toString()), data);
    }
  }
}

export {login, bot};
