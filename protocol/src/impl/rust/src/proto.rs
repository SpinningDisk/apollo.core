#![allow(nonstandard_style)]
#![allow(dead_code)]
use libc;
use crate::socket::*;



#[repr(C)]
pub struct s_Proto_Thread_Info{
    pub id: i32,
    pub io: *const *mut u8,
    pub ready: libc::pthread_mutex_t,
}
#[repr(C)]
pub struct s_Proto{
    pub parser: *mut s_Parser_State,
    pub server: *mut s_Socket,

    pub threads: *mut libc::pthread_t,

    pub ptiPool: *mut s_Proto_Thread_Info,
    pub freePtiSlots: *mut bool,

    pub ready: libc::pthread_mutex_t,

    // methods
    // s0
    _init_pti: unsafe extern "C" fn(pti: *mut s_Proto_Thread_Info) -> *mut s_Proto_Thread_Info,
    _thread: unsafe extern "C" fn(proto: *mut s_Proto, fn_: unsafe extern "C" fn(arg: *mut u8) -> *mut u8, arg: *mut u8) -> i32,
    // s1 functions (closer to bare metal; the higher the simplicity s, the higher level everything gets)
    _free: unsafe extern "C" fn(proto: *mut s_Proto),
    _connect: unsafe extern "C" fn(proto: *mut s_Proto, appName: *const u8),
    _join_thread: unsafe extern "C" fn(proto: *mut s_Proto, index: u32) -> *mut u8,
    _read: unsafe extern "C" fn(proto: *mut s_Proto),
}


#[link(name = "proto")]
unsafe extern "C" {fn init_proto(ptr: u8, name: *const u8) -> *mut s_Proto;}

impl s_Proto{
    pub fn init_pti(&mut self, ptr: Option<*mut s_Proto_Thread_Info>) -> *mut s_Proto_Thread_Info{
        #[allow(unused)] // p will get changed in init_pti. shut up
        let mut p: *mut s_Proto_Thread_Info= ptr.unwrap_or(0 as *mut s_Proto_Thread_Info);
        unsafe{(self._init_pti)(p)}
    }
    pub fn thread(&mut self, fn_: unsafe extern "C" fn(arg: *mut u8) -> *mut u8, arg: *mut u8) -> i32{
        unsafe{(self._thread)(self, fn_, arg)}
    }
}
