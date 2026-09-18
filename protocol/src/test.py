import socket
import sys
from pickle import dumps, loads
import asyncio
import re

from inspect import getmembers, isfunction
import traceback

def send(message, sockname):
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.connect(sockname)
    sock.sendall(message.encode("ascii"))
    sock.close()
class server():
    def __init__(self):
        self.cons = []
        self.server = None
    def panic(self):
        print("running")
        self.server.close()
        os.remove("/mnt/data/dev/botram-boetchen/core/sockets/proto_test.sock")
    async def dispatcher(self, sockname):
        server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        server.bind(sockname)
        self.server = server
        try:
            server.listen(3)
            while True:
                print(f"<<|listening|>>", flush=True)
                con = server.accept()
                asyncio.run(self.listen(con))
        except Exception as E:
            print(f"listening failed with {traceback.print_last()}")
            self.panic()
            # I want this to crash if this fails. if this fails shit has hit the fan
    async def listen(self, con):
        while True:
            msg = await con.recv(2048)
            for i,msg in zip(range(len(recv), recv)):
                print(f"{i}: {msg}")


def lex(string_query):
    while True:
        function_stack = []
        args_stack = []
        query = string_query
        
        # set done to 1 once no more functions can be found in 
        done = 0
        i = 0
        while not done:
            try:
                string_query = query
                queries = re.findall("(\w+)\((.*)\)", string_query)
                for query in queries:
                    function = query.group(1)
                    function_stack.insert(0, function)
                    # replace function arguments with function indices from function_stack (reversed, so 0 is last element so parsing is easier)
                    if len(args_stack)!=0:
                        args_stack[0] = [i if x==string_query else x for x in args_stack]
                    
                    args = query.group(2)
                    args_stack.insert(0, args.strip(' ').split(','))
                    
                query = args
            except Exception:
                break
            finally:
                i += 1            
        print(function_stack, args_stack)

        if query.group(1)=="exit":
            exit()
def naive_lex(string_query):
    query = re.search("(\w+)\((.*)\)", string_query)
    function = query.group(1)
    args = query.group(2)
    args = args.split(',')
    in_string = 0
    for (i,arg) in zip(range(len(args)), args):
        if in_string:
            # if in string, merge the previous element (known to be in string) with current
            args[i-1] = ','.join([args[i-1], args[i]])
            args.pop(i)
        else:
            args[i] = args[i].strip(' ')
            # if argument has multiple quotations (E.g. in print("Hello")->args[0]="Hello"), we check if argument is self closing (by taking rest of count of ")
            x = arg.count('\"')%2
            in_string = x-in_string
            continue
    return [function, args]
def unstring(query):
    if query[0]=="\"" and query[len(query)-1]=="\"":
        return [query[1:len(query)-1], 1]
    else:
        return [query, 0]
def function_switch(function, args, externs):
    match(function):
        case "exit":
            if len(args)>0:
                exit(args[0])
            else:
                exit()
        case "connect":

        case "update":
            try:
                assert len(args)>=2
            except AssertionError:
                print(f"Argument Error@update({', '.join(args)}): not enough elements (expected 2 got {len(args)}({args}))")
                return externs
            externs.update({unstring(args[0])[0]: unstring(args[1])[0]})
            return externs
        case "send":
            try:
                assert len(args)>=1
            except AssertionError:
                print(f"Argument Error@update({', '.join(args)}): not enough elements (expected 1 got {len(args)}({args}))")
                return externs
            send(unstring(args[0])[0], externs['socket_name'])
            return externs
        case "print":
            try:
                assert len(args)>=1
            except AssertionError:
                print(f"Argument Error@print({', '.join(args)}): not enough elements (expected 1 got {len(args)}({args}))")
                return externs
            # collection as in std::stuff being std since I don't know shit
            namespace = "_" if len(args)<2 else unstring(args[0])[0]
            match namespace:
                case "_":
                    print(f"{unstring(args[0])[0]}")
                case "dbg":
                    try:
                        print(externs[unstring(args[1])[0]])
                    except:
                        print(f"Parse Error@{function}({', '.join(args)}): namespace \"{namespace}\" does not contain member \"{unstring(args[1])[0]}")
                case "fnc":
                    try:
                        print([name for name,obj in getmembers(sys.modules[__name__]) if isfunction(obj)])
                    except:
                        print(traceback.format_exc())
                        print(f"Parse Error@{function}({', '.join(args)}): namespace \"{namespace}\" does not contain member \"{unstring(args[1])[0]}")
            return externs
        case "listen":
            externs['mode'] = 'serving'
            recv  = []
            s = server()
            asyncio.run(s.dispatcher(externs['socket_name']))
                    
        case _:
            print(f"Argumetn Error@{function}({', '.join(args)}): function \"{function}\" unknown")
            return externs
if __name__=="__main__":
    print("interactive proto communication console (ipc^2)\ntype help for help")
    mode = "send"
    externs = {
        'mode': 'send', 
        'socket_name':"sockets/proto_test.sock"
    }
    line = 0
    while True:
        try:
            function, args = naive_lex(input(">>"))
            line += 1
        except Exception as E:
            print(f"Syntax Error@l{line}: {E}")
        try:
            externs = function_switch(function, args, externs)
        except Exception as E:
            exc_type, exc_obj, exc_tb = sys.exc_info()
            print(f"Parse Error@{line}: {traceback.format_exc()}")
    """ 
    if sys.argv[1]== "0":
        print("sending")
        send(sys.argv[3], sys.argv[2])
    elif sys.argv[1]=="1":
        print("serving")
        asyncio.start_unix_server()
    """
