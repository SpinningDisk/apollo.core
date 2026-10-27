import ctypes
raise NotImplementedError("need to figure out how to use ctypes")

class Binder:
    def __init__(self):
        ctypes.CDLL("libasan.so", mode=ctypes.RTLD_GLOBAL)
        self.libsocket = ctypes.CDLL("../../../build/lib/libsocket.shared.so", mode=ctypes.RTLD_GLOBAL)
        self.libproto = ctypes.CDLL("../../../build/lib/libproto.shared.so", mode=ctypes.RTLD_GLOBAL)
    def call(self, func, args):
        if type(func) is str:
            try:
                func = getattr(self.libproto, func)
                if not func:
                    raise AttributeError
            except AttributeError:
                sfunc = func.clone()
                func = getattr(self.libsocket, func)
                if not func:
                    raise AttributeError(f"function {sfunc} not found")
        return func(*args)

if __name__ == "__main__":
    binder = Binder()
    print(binder.libsocket)
    print(binder.libproto)
    test = binder.libproto.init_proto(ctypes.c_char_p(b"tests.isolated"))
    test = binder.call("init_proto", (ctypes.c_char_p(b"tests.isolated"),))  ## uups...
