import * as bot from "./bot/bot";

bot.login().then((data) => {
  console.log(`Bot successfully logged in to ${data['username']}#${data['discriminator']}!`);
  bot.bot();
});