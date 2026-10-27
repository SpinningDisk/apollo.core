use serde::{Deserialize, Serialize};
use serde_json;
use tungstenite::http::{Method, Request};
use tokio_tungstenite::connect_async;
#[cfg(target_os="linux")]
use std::os::unix::net::{UnixListener, UnixStream};
#[cfg(target_os="windows")]
use std::os::windows::net::{UnixListener, UnixStream};
use tokio;
use std::fs::File;
use std::io::prelude::*;
use std::env;
use std::error::Error;


pub fn get_project_dir() -> Result<String, Box<dyn Error>>{
    let mut project_dir = env::var("APOLLO_SOURCE_DIR")?;

    // let mut project_dir: Result<String, std::env::VarError> = 
    match std::env::var("APOLLO_SOURCE_DIR") {
        Ok(value)=> Ok(value.to_owned()),
        Err(std::env::VarError::NotPresent)=> {
            println!("\x1b[31;1mERROR: NET.rs could not find project directory... sowy 3:\ndumped log where you are\x1b[0m"); 
            let mut f = File::create("netrs.log")?;
            let e_bytes = format!("PythonSocks@connect: retrieving \"APOLLO_SOURCE_DIR\"");
            f.write_all(e_bytes.as_bytes()).expect("Failed to write");
            for pair in std::env::vars(){
                let pair_as_string= format!("\t{}:{}\n", pair.0, pair.1);
                f.write_all(pair_as_string.as_bytes()).expect("Failed to write");  
            }
            return Err(Box::new(std::env::VarError::NotPresent));
        },
        Err(e)=> {
            println!("\x1b[31;1mERROR: what in mothers sweet child did you just do now?🤨 (environmetn Variables are not UTF8 encoded)");
            return Err(Box::new(e))
        }
    }

    // if project_dir==VarErr
    // let relative_sock_path: &str= "/NETRS-PY.sock";
    // let mut sock_path = project_dir.unwrap() + relative_sock_path;
    // UnixListener::bind(sock_path);

    // Ok(())
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Ping{
    source: String,
    port: u32,
    data: serde_json::Value
}
pub struct PythonSocks{
    sock: Option<UnixListener>,
}
impl Default for PythonSocks{
    fn default() -> Self{
        Self{
            sock: None   
        }
    }
}
impl PythonSocks{
    pub fn open(&mut self) -> Result<UnixListener, Box<dyn Error>>{
        let path: &str = &(get_project_dir().unwrap().to_string() + "/socks/net_rs->py.sock");
        let server = UnixListener::bind(path)?;
        Ok(server)
        // NOTICE: panic (UnixListener::bind)
        // Err(Box::new(std::env::VarError::NotPresent))
    }
    pub async fn unsafe_accept_next(&mut self) -> Option<UnixStream>{
        let mut sock = self.sock.unwrap();
        match sock.accept(){
            Ok((listener, addr))=>{return Some(listener)},
            Err(e)=>{println!("[NET_RS]@PythonSocks.unsafe_accept_next\tunknown error at accept: {}", e); return None},
        }
    }
    pub async fn accept_next(&mut self) -> Option<UnixStream>{
        match self.sock{
            None=> {
                println!("[NET.RS]@PythonSocks.accept_next\taccept_next called before open. this is the end, I'm affraid");
                panic!("accept_next called with no socket");
            },
            Some(UnixListener)=>{}
        }
        let mut sock = self.sock.unwrap();
        match sock.accept(){
            Ok((listener, addr))=>{return Some(listener)},
            Err(e)=>{println!("[NET_RS]@PythonSocks.unsafe_accept_next\tunknown error at accept: {}", e); return None},
        }
    }
    pub async fn serve(&mut self) -> (){
        match self.sock{
            None=> {
                println!("[NET.RS]@PythonSocks.serve\tserve called before open. this is the end, I'm affraid");
                panic!("servecalled with no socket");
            },
            Some(UnixListener)=>{}
        }
        for stream in self.sock.unwrap().incoming(){
            match stream{
                Ok(mut stream)=>{
                    
                },
                Err(e)=>{
                    println!("[NET.RS]@PythonSocks.serve\tincomming stream err: {}", e);
                }
            }
        }
    }
    pub async fn retrieve(&mut self) -> (){
        let mut server = self.accept_next();   
    }
}


#[cfg(test)]
mod tests {
    use std::os::unix::net::UnixStream;
    use std::io::prelude::*;
    
    use super::*;
    
    #[test]
    fn simulate_ping_pong(){
        ()
    }
    #[test]
    fn test_socket(){
        let mut ps = PythonSocks::default();
        ps.connect();
    }
}
