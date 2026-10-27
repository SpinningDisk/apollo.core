#![allow(nonstandard_style)]
use libc;

#[repr(C)]
pub enum e_Parser_Token_Types{
    // msgpack  signals
    SOCKET_HEAD_HEADER              = 0x00,  // nil

    SOCKET_HEAD_FINISH              = 0x01,  // some special sauce
    SOCKET_HEAD_FALSE               = 0x02,  // false
    SOCKET_HEAD_TRUE                = 0x03,  // true

    SOCKET_HEAD_BIN_8               = 0x04,
    SOCKET_HEAD_BIN_16              = 0x05,
    SOCKET_HEAD_BIN_32              = 0x06,

    SOCKET_HEAD_EXT_8               = 0x07,
    SOCKET_HEAD_EXT_16              = 0x08,
    SOCKET_HEAD_EXT_32              = 0x09,

    SOCKET_HEAD_FLOAT               = 0x0a,
    SOCKET_HEAD_DOUBLE              = 0x0b,
    SOCKET_HEAD_UINT_8              = 0x0c,
    SOCKET_HEAD_UINT_16             = 0x0d,
    SOCKET_HEAD_UINT_32             = 0x0e,
    SOCKET_HEAD_UINT_64             = 0x0f,
    SOCKET_HEAD_INT_8               = 0x10,
    SOCKET_HEAD_INT_16              = 0x11,
    SOCKET_HEAD_INT_32              = 0x12,
    SOCKET_HEAD_INT_64              = 0x13,

    SOCKET_HEAD_FIXEXT_1            = 0x14,
    SOCKET_HEAD_FIXEXT_2            = 0x15,
    SOCKET_HEAD_FIXEXT_4            = 0x16,
    SOCKET_HEAD_FIXEXT_8            = 0x17,
    SOCKET_HEAD_FIXEXT_16           = 0x18,

    SOCKET_HEAD_STR_8               = 0x19, // str8
    SOCKET_HEAD_STR_16              = 0x1a, // str16
    SOCKET_HEAD_STR_32              = 0x1b, // str32
    SOCKET_HEAD_ARRAY_16            = 0x1c,
    SOCKET_HEAD_ARRAY_32            = 0x1d,
    SOCKET_HEAD_MAP_16              = 0x1e,
    SOCKET_HEAD_MAP_32              = 0x1f,

    // custom
    SOCKET_HEAD_EMPTY               = 0x20,
}
#[repr(C)]
pub struct s_Parser_Token{
    pub tok_type: e_Parser_Token_Types,
    pub content: *mut u8,
    pub empty: isize,
    pub size: isize,
    pub dynamic: bool,
}
#[repr(C)]
pub struct s_Parser_State{
    pub pos: *mut u8,
    pub tokens: *mut s_Parser_Token,
    pub tokenCount: usize,
    pub tokensAllocated: usize,
    pub parse: unsafe extern "C" fn(bytes: *mut u8) -> *mut s_Parser_State,
}
#[repr(C)]
pub enum e_Status{
    // "normal codes"
    OK,
    HEARTBEAT,
    NOT_MODIFIED,

    // User definied
    SIG_0,
    SIG_1,
    SIG_2,
    SIG_3,
    SIG_4,
    SIG_5,
    SIG_6,
    SIG_7,
    SIG_8,
    SIG_9,

    // Error
    INVALID_SCHEME,
    FORBIDDEN,
    REJECTED,
    NOT_FOUND,
    INTERNAL_ERROR,
    TIMEOUT,
}
#[repr(C)]
pub struct s_Response{
    pub sig: e_Status,
    pub message: *mut u8,
    pub MessageLength: usize,
}
#[repr(C)]
pub struct s_Socket{
    pub socketFd: u32,
    pub addr: libc::sockaddr_un,
    pub path: *mut u8,
    pub io: *mut *mut u8,

    pub get: unsafe extern "C" fn(sock: *mut s_Socket) -> *mut s_Response,
    pub send: unsafe extern "C" fn(sock: *mut s_Socket, bytes: *mut u8) -> e_Status,
}

#[link(name = "socket")]
unsafe extern "C" {
    pub fn init_server(ptr: u8, name: *const u8, ports: u16) -> *mut libc::c_void;
    pub fn init_client(client: u8, name: *const u8, port: u16) -> *mut libc::c_void;
}

