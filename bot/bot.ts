import { exit } from "process";
import { net,scrapeToken } from "./discord/discord"
import { parser } from "./parse/parser"
import { deflateSync } from "zlib"
import * as ws from "ws"

let botConfig: string[] = parser.confToJson("bot/bot.conf");
let TOKEN: string;
let userData = {};

async function login() {
  if (botConfig["SCRAPETOKEN"] == true) {
    let s:string = scrapeToken();
    if (s == null){
      console.log("Token couldn't be scraped.");
    }
  } if (botConfig["LOGINUSER"] != null && botConfig["LOGINPASSWORD"] != null && !TOKEN) {
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
    console.log("No token or method to get token was given or worked. Edit bot/bot.conf");
    exit(1);
  }

  let data = JSON.parse((await (await net.sendApiReq("users/@me", Buffer.from(""), "GET", TOKEN)).toString()));
  userData = data;

  return data;
}

function bot() {
  // MAKE THIS WORK
  let sock = ws("wss://gateway.discord.gg/?encoding=json&v=9&compress=zlib-stream")
}

export {login, bot};
