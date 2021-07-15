import { exit } from "process";
import { net,scrapeToken } from "./discord/discord"
import { parser } from "./parse/parser"
import * as WebSocket from "ws"

let botConfig: string[] = parser.confToJson("bot/bot.conf");
let TOKEN: string;
let userData = {};
let client = {};

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
        case 10:
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
      switch (m["t"]) {
        case "READY":
          client = m["d"];
          console.log(`Client ready (${client['user']['username']}#${client['user']['discriminator']}).`);
          break;
        case "READY_SUPPLEMENTAL":
          break;
        case "SESSIONS_REPLACE":
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

export {login, bot};
