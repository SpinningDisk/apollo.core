use net_rs;
use std::os::unix::net::UnixListener;

fn main()->(){
    let mut ps = net_rs::PythonSocks::default();
    let server = ps.open();
   println!("created socket @ {}/socks/net_rs->py.sock", net_rs::get_project_dir().unwrap());
    while(true){
        print!("");
    }
}
