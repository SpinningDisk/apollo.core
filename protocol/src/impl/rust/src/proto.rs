#![allow(nonstandard_style)]
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
    pub init_pti: unsafe extern "C" fn(pti: *mut s_Proto_Thread_Info) -> *mut s_Proto_Thread_Info,
    pub thread: unsafe extern "C" fn(proto: *mut s_Proto, fn_: unsafe extern "C" fn(arg: *mut u8) -> *mut u8, arg: *mut u8) -> i32,
    // s1 functions (closer to bare metal; the higher the simplicity s, the higher level everything gets)
    pub free: unsafe extern "C" fn(proto: *mut s_Proto),
    pub connect: unsafe extern "C" fn(proto: *mut s_Proto, appName: *const u8),
    pub join_thread: unsafe extern "C" fn(proto: *mut s_Proto, index: u32) -> *mut u8,
    pub read: unsafe extern "C" fn(proto: *mut s_Proto),
}


#[link(name = "proto")]
unsafe extern "C" {
    pub fn init_parser(ptr: u8) -> *mut s_Parser_State;
    pub fn free_parser(parser: *mut s_Parser_State);

    pub fn init_pti(pti: u8) -> *mut s_Proto_Thread_Info;
    pub fn free_pti(pti: *mut s_Proto_Thread_Info);

    pub fn init_proto(ptr: u8, name: *const u8) -> *mut s_Proto;
    pub fn free_proto(proto: *mut s_Proto);
}
