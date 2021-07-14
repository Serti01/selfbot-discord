import { net } from "./discord/discord"
import { parser } from "./parse/parser"

let botconfig:string[] = parser.confToJson("bot/bot.conf");
let TOKEN:string;

function login() {
  if (botconfig["LOGINUSER"] != null && botconfig["LOGINPASSWORD"] != null) {
    let netr:Promise<string> = net.sendApiReq("auth/login", Buffer.from(JSON.stringify({
      login:botconfig["LOGINUSER"],
      password:botconfig["LOGINPASSWORD"],
      undelete:false,
      captcha_key:null,
      login_source:null,
      gift_code_sku_id:null,
    })));
    let data;
    netr.then((v) => {
      data = JSON.parse(v.toString());
      TOKEN = data["token"];
    });
  } else if (botconfig["TOKEN"] != null) {
    let netr:Promise<string> = net.sendApiReq("users/@me", Buffer.from(""), "GET", botconfig["TOKEN"]);
    let data;
    netr.then((v) => {
      data = JSON.parse(v.toString());
      if (data["id"] != undefined && data["username"] != undefined && data["discriminator"] != undefined) {
        TOKEN = botconfig["token"];
      }
      console.log(data);
    })
  } else {
    // no way to get the token was given, ask.
  }

  console.log("TOKEN",TOKEN);
}

export { login };