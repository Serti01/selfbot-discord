import { net } from "./discord/discord"
import { parser } from "./parse/parser"

let botconfig:string[] = parser.confToJson("bot/bot.conf");
let TOKEN:string;

function login() {
  if (botconfig["LOGINUSER"] != null && botconfig["LOGINPASSWORD"] != null) {
    // login to discord with the api to get the token
  } else if (botconfig["TOKEN"] != null) {
    // confirm token
  } else {
    // no way to get the token was given, ask.
  }
}