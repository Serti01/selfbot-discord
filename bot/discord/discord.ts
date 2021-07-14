import { request, RequestOptions } from "https";

class net {
  async sendApiReq(api:string, body:Buffer = Buffer.from(""), method:string = "GET", auth?:string):Promise<string> {
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

    return new Promise<string>((pres,prej) => {
      let req = request(options, res => {
        res.on("data", async (d) => {
          (await (async function() {return new Promise<void>((res,rej)=>{setTimeout(()=>{res()},1000)})})());
          pres(d);
        });
      });

      req.on("error", (e) => {
        console.error(e);
      });

      req.write(body);
      req.end();
    });
  }
}

/*const https = require('https')

const data = new TextEncoder().encode(
  JSON.stringify({
    todo: 'Buy the milk 🍼'
  })
)

const options = {
  hostname: 'whatever.com',
  port: 443,
  path: '/todos',
  method: 'POST',
  headers: {
    'Content-Type': 'application/json',
    'Content-Length': data.length
  }
}

const req = https.request(options, res => {
  console.log(`statusCode: ${res.statusCode}`)

  res.on('data', d => {
    process.stdout.write(d)
  })
})

req.on('error', error => {
  console.error(error)
})

req.write(data)
req.end()*/

let _net = new net();

export { _net as net };