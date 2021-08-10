import { readFileSync } from "fs";

class parser {
  readConf(file:string): string[] {
    let filebuffer:Buffer = readFileSync(file);
    let data:string = filebuffer.toString();
  
    let statements:string[] = [];
    let _statements:string[] = data.split(/\n/).map(
      ((v) => {
        if (v == "" || v[0] == ";") return;
        
        return v;
      })
    );
  
    for (let i = 0; i < _statements.length; i++) {
      if (_statements[i] != undefined) statements.push(_statements[i]);
    }
  
    return statements;
  }

  confToJson(file:string): string[] {
    let conf:string[] = this.readConf(file);
    
    let json = [];
    let jsons = "{";

    for (let i = 0, entry = conf[i]; i < conf.length; i++, entry = conf[i]) {
      let data = entry.split(/=/,2);
      
      try {
        jsons += `"${data[0]}":${data[1]},`;
        //json.push(JSON.parse(`{"${data[0]}":${data[1]}}`));
      } catch (e) {
        console.log(e);
      }
    }
    
    //use json as a tmp variable here
    json = jsons.split('');
    json[json.length-1] = "}";
    jsons = json.join('');
    json = JSON.parse(jsons);
    
    return json;
  }
}

let _parser = new parser();

export { _parser as parser };