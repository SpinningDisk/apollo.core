use libc;
mod socket;
mod proto;
use crate::proto::*;
use crate::socket::*;


fn main()->() {
    let name: &[u8] = b"tests.isolated\0";
    let proto = unsafe {init_proto(0, name.as_ptr()) };
    println!("proto@{:?}", proto);
    unsafe{((*proto).free)(proto)};
    ()
}

