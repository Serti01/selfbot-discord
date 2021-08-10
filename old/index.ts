import { exit } from "process";
import * as bot from "./bot/bot";

bot.login().then(async () => {
  console.log(`Login success, starting bot`);
  bot.bot();
  // this is so it doesnt exit;
  while (true) {
    await new Promise<void>(res => {setTimeout(res,1000)});
  }
});