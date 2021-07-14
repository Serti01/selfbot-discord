import { exit } from "process";
import { net } from "./discord/discord"
import { parser } from "./parse/parser"

let botConfig: string[] = parser.confToJson("bot/bot.conf");
let TOKEN: string;
let userData = {};

async function login() {
  if (botConfig["LOGINUSER"] != null && botConfig["LOGINPASSWORD"] != null) {
    let data = JSON.parse((await net.sendApiReq("auth/login", Buffer.from(JSON.stringify({
      login: botConfig["LOGINUSER"],
      password: botConfig["LOGINPASSWORD"],
      undelete: false,
      captcha_key: null,
      login_source: null,
      gift_code_sku_id: null
    })),"POST")).toString());
    TOKEN = data["token"];
  } else if (botConfig["TOKEN"] != null) {
    let b = await net.sendApiReq("users/@me",null,null,botConfig["TOKEN"]);
    let data = JSON.parse(b.toString());
    if (data["message"] != "401: Unauthorized") {
      TOKEN = botConfig["TOKEN"];
    }
  } else {
    console.log("No token or method to get token was given. Edit bot/bot.conf");
    exit(1);
  }

  let data = JSON.parse((await (await net.sendApiReq("users/@me", Buffer.from(""), "GET", TOKEN)).toString()));
  userData = data;

  return data;
}

function bot() {

}

export {login, bot};
