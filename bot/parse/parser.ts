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

    for (let i = 0, entry = conf[i]; i < conf.length; i++, entry = conf[i]) {
      let data = entry.split(/=/,2);
      
      try {
        json.push(JSON.parse(`{"${data[0]}":${data[1]}}`));
      } catch (e) {
        console.log(e);
      }
    }

    return json;
  }
}

let _parser = new parser();

export { _parser as parser };