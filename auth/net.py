import requests, asyncio, aiohttp, socket, websockets, discord, secrets, random, sys, os, subprocess, dotenv, string, re, time, traceback
from inspect import currentframe, getframeinfo
from dataclasses import dataclass

dotenv.load_dotenv()

def cli_Message(message, offset=0):
    sys.stdout.write(f"\x1b[{len(message.split("\n"))+1+offset}A")
    sys.stdout.write("\x1b[J") 
    sys.stdout.write(message)
    sys.stdout.flush()
class NetManager():
    @staticmethod
    async def Start_Up(verbose):
        NM = NetManager()
        T = NM.TunnelManager()
        await T._Get_Cloudflared_Url(verbose)
        url, secret = T.url, T.secret
        del(T)
        del(NM)
        return url, secret
    @staticmethod
    async def Close(url, verbose):
        NM = NetManager()
        # let clients finish up
        await asyncio.sleep(5)
        await NM.DiscordManager.Close(url, verbose)
    
    # Class for handeling discord rondevouz (server+client)
    class DiscordManager():
        color = "\033[32m"
        color_important = "\033[32;1m"
        clear_color = "\033[0m"
        # class methods since I want to call NM.DM.<method>() without first instanciating every class
        @staticmethod
        async def Server_Up(url, verbose=False):
            try:
                if verbose:
                    print(f"{color}[DM]\tsending url{clear_color}")
                async with aiohttp.ClientSession() as session:
                    web_hook = discord.Webhook.from_url(url=os.getenv("DISCORD_WEBHOOK_URL"), session=session)
                    secret = ''.join([random.choice([str(secrets.randbits(random.randint(1, 16))), string.ascii_letters[random.randint(0, len(string.ascii_letters)-1)]]) for _ in range(int(round(1024/16)))])
                    dic = {
                        "title": "SERVERUP",
                        "fields": (
                            {"name": "stat", "value": "up"},
                            {"name": "url", "value": url},
                            {"name": "time", "value": str(time.time())},
                            {"name": "secret", "value": secret}
                        )
                    }
                    embeds = discord.Embed.from_dict({
                        "title": "SERVERUP",
                        "fields": (
                            {"name": "stat", "value": "up"},
                            {"name": "url", "value": url},
                            {"name": "time", "value": str(time.time())},
                            {"name": "secret", "value": secret}
                        )
                    })
                    await web_hook.send(username="SERVER@apollo", embeds=[embeds])
                    return secret
            except Exception as E:
                        summary = traceback.StackSummary.extract(
                            traceback.walk_stack(None)
                        )
                        print(f"{NetManager.DiscordManager.color_important}[DM]\tEXC@{inspect.FrameInfo.function}x{inspect.FrameInfo.lineno}: \n{''.join(summary.format())}\n\tDM set to Panic{NetManager.DiscordManager.clear_color}")
                        await NetManager.DiscordManager.Panic(url)
        @staticmethod
        async def Panic(url):
            async with aiohttp.ClientSession() as session:
                web_hook = discord.Webhook.from_url(url=os.getenv("DISCORD_WEBHOOK_URL"), session=session)
                embeds = discord.Embed.from_dict({
                    "title": "SERVERPANIC",
                    "fields": (
                        {"name": "stat", "value": "panic"},
                        {"name": "url", "value": url},
                    )
                })
                await web_hook.send(username="SERVER@apollo", embeds=[embeds])
        @staticmethod
        async def Close(url, verbose=False):
            try:
                if verbose:
                    print(f"{NetManager.DiscordManager.color}[DM]\tclosing! Bye :3{NetManager.DiscordManager.clear_color}")
                async with aiohttp.ClientSession() as session:
                    web_hook = discord.Webhook.from_url(url=os.getenv("DISCORD_WEBHOOK_URL"), session=session)
                    embeds = discord.Embed.from_dict({
                        "title": "Close",
                        "fields": (
                            {"name": "stat", "value": "closed"},
                            {"name": "url", "value": url},
                            {"name": "time", "value": str(time.time())},
                        )
                    })
                    await web_hook.send(username="SERVER@apollo", embeds=[embeds])
            except Exception as E:
                        summary = traceback.StackSummary.extract(
                            traceback.walk_stack(None)
                        )
                        print(f"{NetManager.DiscordManager.color_important}[DM]\tEXC@: \n{''.join(summary.format())}\n\tDM set to Panic with {E}{NetManager.DiscordManager.clear_color}")
                        await NetManager.DiscordManager.Panic(url)
        @staticmethod   # TODO
        async def Get_Clients(verbose=False):
            intents = discord.Intents.default()
            intents.message_content = True
            self.client = discord.Client(intents=intents)
            self.context_length = 50


        # client classmethods
        @staticmethod
        def Huston(message, iterable_amount, iterable=None): 
            if os.name=="nt":
                file_path="img\\no_connection\\"
            else:
                file_path="img/no_connection/"
            for frame,i in zip(list(os.walk(os.getenv("PROJECT_DIR")+file_path))[0][2], range(len(list(os.walk(os.getenv("PROJECT_DIR")+file_path))[0][2]))):
                if not frame.endswith(".ascii"):
                    continue
                with open(os.getenv("PROJECT_DIR")+file_path+frame, "r") as f:
                    cli_Message(f"Houston, we've got a problem...\n{f.read()}\n{message}{''.join([iterable for _ in range(i//(42//iterable_amount)) if iterable is not None])}\n", offset=((i+1)//(42//iterable_amount))) #no idea what the offset formula be doin' here. did this months ago
                    time.sleep(1/10)
        # this crashes for unexplainable reasons
        @staticmethod
        async def Create_Client():
            dis = NetManager.DiscordManager("client")
            task1 = asyncio.create_task(dis.client.start(os.getenv("DISCORD_BOT")))
            task2 = asyncio.create_task(dis._Send_Auth_Request())
            await task1
            await task2
            return
        @staticmethod
        async def Create_Client():

        def __init__(self, user):
            if user=="server" or user=="both":
                self.wh_url = os.getenv("DISCORD_WEBHOOK_URL")
                self.url = None
                self.secret = None
                
                intents = discord.Intents.default()
                intents.message_content = True
                self.client = discord.Client(intents=intents)
                self.context_length = 50
            elif user=="client" or user=="both":
                intents = discord.Intents.default()
                intents.message_content = True
                self.client = discord.Client(intents=intents)
                self.channel = None
                self.secret = None
                self.url = None
                self.con_info = None
                self.context_length = 50
                self.server_message_validity_time = 2*24*60*60
                adverb_list = ["paticularly", "slyly", "shyly", "plottingly", "observingly", "happyly", "errotically", "fried", "tiredly", 
                "friendlily", "cooked", "failingly", "trans", "magnificantly", "fastly", "easily", "sweatingly", "competitevely", 
                "industrially", "efficiently", "greatly", "officially", "openly", "arousingly", "frenchly", "famously", "obviously", 
                "fanatically", "romantically", "lustfully", "jokingly", "not-so-obviously", "wholeheartedly"]
                verb_list = ["functioning", "attending", "baking", "laughing", "processing", "programming", "hacking", "partying",
                "crying", "dating", "loving", "hating", "fighting", "peace-making", "training", "fishing", "cursing", "cooking", "frying",
                "developing", "guessing", "learning", "researching", "studying", "dooming", "drooling", "starving", "drowning",
                "flourishing", "tempting", "dying", "hoping", "praying", "killing", "slaying", "gaming", "gayming", "typing", "gendering",
                "transitioning", "dominating", "submitting"]
                noun_list = ["queen", "king", "Guacamole", "demon", "gremlin", "egg", "freelancer", "femboy", "tomboy", "api-key",
                "bot", "server", "graph", "hypervisor", "interpreter", "compiler", "soda", "soda-pop", "rust-programmer", "arch-user", 
                "klemmbaustein", "rindfleischettiketierungsaufgabenüberwachungsgesetz"]
                self.internal_name = f"{random.choice(adverb_list)} {random.choice(verb_list)} {random.choice(noun_list)}"
            if user=="server":
                @self.client.event
                async def on_ready():
                    self.channel = await self.client.fetch_channel(int(os.getenv("DISCORD_CHANNEL")))
                    # loop through every message in our context window, if there's no embeds we don't touch it, otherwise check if we hit out opening message and break the loop or register a user if the embed contains a user
                    while True:
                        await asyncio.sleep(0.2)
                        async for msg in self.channel.history(limit=self.context_length):
                            if len(msg.embeds)<=0:
                                continue
                            data = msg.embeds[0]
                            if data.tile=="SERVERUP":
                                # break
                                pass
                            elif data.title=="CLIENTAUTH":
                                print(data)
            dis = NetManager.DiscordManager("client")
            task1 = asyncio.create_task(dis.client.start(os.getenv("DISCORD_BOT")))
            task2 = asyncio.create_task(dis._Send_Auth_Request())
            await task1
            await task2
            return
            
            if user=="client":
                @self.client.event
                async def on_ready():
                    self.channel = await self.client.fetch_channel(int(os.getenv("DISCORD_CHANNEL")))
                    while True:
                        await asyncio.sleep(0.2)
                        server_is_online = await self.Check_Messages()
                        print(f"client@onready: server is online? {server_is_online}")
                        if server_is_online:
                            pass
                            # await self.client.close()
                        else:
                            self.Huston("Server does not seem to be online yet...\nYou're on your own", 3, ".")
                @self.client.event
                async def on_message(msg):
                    SPINNIDM = 1443289897099198514
                    if message.author.id == client.user.id:
                        # bot message
                        return
                    if message.channel.id == SPINNIDM and message.content.lower()=="close":
                        await self.client.close()

        # server methods
        async def _Server_Up(self, url):
            self.url = url
            async with aiohttp.ClientSession() as session:
                web_hook = discord.Webhook.from_url(url=self.wh_url, session=session)
                self.secret = ''.join([random.choice([str(secrets.randbits(random.randint(1, 16))), string.ascii_letters[random.randint(0, len(string.ascii_letters)-1)]]) for _ in range(int(round(1024/16)))])
                embeds = discord.Embed.from_dict({
                    "title": "SERVERUP",
                    "fields": (
                        {"name": "stat", "value": "up"},
                        {"name": "url", "value": url},
                        {"name": "time", "value": str(time.time())},
                        {"name": "secret", "value": self.secret}
                    )
                })
                await web_hook.send(username="SERVER@apollo", embeds=[embeds])
        async def _Panic(self, url):
            async with aiohttp.ClientSession() as session:
                web_hook = discord.Webhook.from_url(url=os.getenv("DISCORD_WEBHOOK_URL"), session=session)
                embeds = discord.Embed.from_dict({
                    "title": "SERVERPANIC",
                    "fields": (
                        {"name": "stat", "value": "panic"},
                        {"name": "url", "value": url},
                    )
                })
                await web_hook.send(username="SERVER@apollo", embeds=[embeds])
        async def _Close(self, url, verbose):
            if verbose:
                print(f"{color}[DM]\tsending url{clear_color}")
            async with aiohttp.ClientSession() as session:
                web_hook = discord.Webhook.from_url(url=os.getenv("DISCORD_WEBHOOK_URL"), session=session)
                embeds = discord.Embed.from_dict({
                    "title": "Close",
                    "fields": (
                        {"name": "stat", "value": "closed"},
                        {"name": "url", "value": url},
                        {"name": "time", "value": str(time.time())},
                    )
                })
                await web_hook.send(username="SERVER@apollo", embeds=[embeds])
        
        
        # Client methods
        def _Huston(self, message, iterable_amount, iterable=None): 
            if os.name=="nt":
                file_path="img\\no_connection\\"
            else:
                file_path="img/no_connection/"
            for frame,i in zip(list(os.walk(os.getenv("PROJECT_DIR")+file_path))[0][2], range(len(list(os.walk(os.getenv("PROJECT_DIR")+file_path))[0][2]))):
                if not frame.endswith(".ascii"):
                    continue
                with open(os.getenv("PROJECT_DIR")+file_path+frame, "r") as f:
                    cli_Message(f"Houston, we've got a problem...\n{f.read()}\n{message}{''.join([iterable for _ in range(i//(42//iterable_amount)) if iterable is not None])}\n", offset=((i+1)//(42//iterable_amount)))
                    time.sleep(1/12)
        async def _Check_Messages(self, verbose=False):
            try:
                self.channel = await self.channel
            except:
                pass
            skip_urls = []
            async for msg in self.channel.history(limit=self.context_length):
                if verbose:
                    print(f"{self.color}[DM]\tgot message {msg}{self.clear_color}")
                if len(msg.embeds)<=0:
                    continue
                data = msg.embeds[0]
                if verbose:
                    print(f"{self.color}[DM]\tgot embeds {data}{self.clear_color}")
                match data.title:
                    case "SERVERUP":
                        for field in data.fields:
                            if field.name=="state" and field.value!="up":
                                print(f"{self.color_important}[DM] unknown state {field.value} for field \"state\"{self.clear_color}")
                                exit() # TODO: gracefully exit/save
                            elif field.name == "url" and field.name not in skip_urls:
                                if verbose:
                                    print(f"{self.color}[DM]\tgot tunnel@{url}{self.clear_color}")
                                self.url = field.value
                                self.con_info = dict(zip([field.name for field in data.fields], [field.value for field in data.fields]))
                            elif field.name == "secret":
                                self.secret = field.value
                            elif field.name=="time":
                                if time.time()-float(field.value)>self.server_message_validity_time:
                                    if verbose:
                                        print(f"{self.color}[DM]\tgot out-dated embed {data} ({time.time()-float(field.value)}s too old)")
                                    self.url = None
                                    self.con_info = None
                                    self.secret = None
                                else:
                                    pass
                    case "SERVERDOWN":
                        for field in data.fields:
                            if field.name=="time" and time.time()-float(field.value)<self.server_message_validity_time:
                                if verbose:
                                    print(f"{self.color}[DM]\tskipping point")
                                skip_urls.append([f.name for f in data.fields if f.name=="url"][0])
                    case _:
                        if verbose:
                            print(f"{self.color}[DM]\tuh! something exotic! dang... don't even know how to handle this...{self.clear_color}")
                if self.url:
                    return 1
        async def _Send_Auth_Request(self):
            while not self.channel:
                await asyncio.sleep(1)
                continue
            # msg = discord.Message(ConnectionState=None, channel=self.channel, data=discord.MessagePayload())
            embed = discord.Embed.from_dict({
                "title": "CLIENTAUTH",
                "fields": (
                    {"name": "internal_name", "value": self.internal_name},
                )
            })
            await self.channel.send(embed=embed)
            await self.client.close()
        async def _Create_Client(self):
            task1 = asyncio.create_task(self.client.start(os.getenv("DISCORD_BOT")))
            task2 = asyncio.create_task(self._Send_Auth_Request())
            await task1
            await task2
            return
    class TunnelManager():
        color = "\033[30m"
        color_important = "\033[30;1m"
        color_clear = "\033[0m"
        
        @dataclass(init=False)
        class Handels:
            async def ping(ws):
                data = json.loads({"PORTS": {1: "PONG"}})

        @staticmethod
        async def Get_Cloudflared_Url(verbose):
            if verbose:
                print(f"{NetManager.TunnelManager.color}[CFD]\tentering{NetManager.TunnelManager.color_clear}")
            cloudflared = await asyncio.create_subprocess_exec(
                "cloudflared",
                "tunnel",
                "--url",
                f"localhost:{int(os.getenv("TUNNEL_LOCAL_PORT"))}",
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
            )
            assert cloudflared.stdout is not None
            # TODO: sleep after kill to make sure we don't kill the new cf process
            async for raw_line in cloudflared.stdout:
                line = raw_line.decode().strip()
                match = re.compile("https://([a-zA-Z0-9.-]+\.trycloudflare\.com)").search(line)
                if match:
                    host = match.group(1)
                    wss_url = f"wss://{host}"
                    if verbose:
                        print(f"{NetManager.TunnelManager.color}[CFD]\tFOUND: {wss_url}{NetManager.TunnelManager.color_clear}")
                    # wait for server to get up; don't want to ping a server that's down now, would we?
                    print("starting timer")
                    await asyncio.sleep(10)
                    print("finished timer")
                    try:
                        async with websockets.connect(wss_url) as ws:
                            await ws.send(json.dumps({"SERVER": {"type": "SERVERSTATUSTEST"}}))
                    except socket.gaierror:
                        if verbose:
                            print(f"{NetManager.TunnelManager.color_important}[CFD]\tcloudflare free tunnel instability made phantom-domain... retrying {NetManager.TunnelManager.color_clear}")
                        subprocess.Popen(["killall", "cloudflared"])
                        url,secret = await NetManager.TunnelManager.Get_Cloudflared_Url(verbose)
                        return url,secrets
                    except ConnectionResetError:
                        if verbose:
                            print(f"{NetManager.TunnelManager.color_important}[CFD]\tcloudflare free tunnel instability made dying-domain... retrying {NetManager.TunnelManager.color_clear}")
                        url,secret = await NetManager.TunnelManager.Get_Cloudflared_Url(verbose)
                        return url,secrets
                    except Exception as E:
                        if verbose:
                            print(f"{NetManager.TunnelManager.color_important}[CFD]\tcloudflare free tunnel instability made phantom-domain with {E}... retrying {NetManager.TunnelManager.color_clear}")
                        subprocess.Popen(["killall", "cloudflared"])
                        hans = await NetManager.TunnelManager.Get_Cloudflared_Url(verbose)
                        print(hans)
                        summary = traceback.StackSummary.extract(
                            traceback.walk_stack(None)
                        )
                        print(''.join(summary.format()))
                        exit()
                        return url,secret
                    if verbose:
                        print(f"{NetManager.TunnelManager.color_important}[CFD]\tFOUND: {wss_url}{NetManager.TunnelManager.color_clear}")
                    url = wss_url
                    secret = await NetManager.DiscordManager.Server_Up(wss_url)
                    return url, secret
        @staticmethod
        def Close(url=None):
            asyncio.run(NetManager.DiscordManager(url))
            subprocess.Popen(["killall", "cloudflared"])
            return 0
        def __init__(self, url=None):
            self.url = url
            self.ws = None
        async def _Get_Cloudflared_Url(self, verbose):
            if verbose:
                print(f"{self.color}[CFD]\tentering{self.color_clear}")
            cloudflared = await asyncio.create_subprocess_exec(
                "cloudflared",
                "tunnel",
                "--url",
                f"localhost:{int(os.getenv("TUNNEL_LOCAL_PORT"))}",
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
            )
            assert cloudflared.stdout is not None
            
            async for raw_line in cloudflared.stdout:
                line = raw_line.decode().strip()
                match = re.compile("https://([a-zA-Z0-9.-]+\.trycloudflare\.com)").search(line)
                if match:
                    host = match.group(1)
                    wss_url = f"wss://{host}"
                    if verbose:
                        print(f"{self.color}[CFD]\tFOUND: {wss_url}{self.color_clear}")
                    # wait for server to get up; don't want to ping a server that's down now, would we?
                    await asyncio.sleep(10)
                    try:
                        async with websockets.connect(wss_url) as ws:
                            await ws.send(json.dumps({"SERVER": {"type": "SERVERSTATUSTEST"}}))
                    except socket.gaierror:
                        if verbose:
                            print(f"{self.color_important}[CFD]\tcloudflare free tunnel instability made phantom-domain... retrying {self.color_clear}")
                        subprocess.Popen(["killall", "cloudflared"])
                        await self._Get_Cloudflared_Url(verbose)
                    except ConnectionResetError:
                        if verbose:
                            print(f"{self.color_important}[CFD]\tcloudflare free tunnel instability made dying-domain... retrying {self.color_clear}")
                        await self._Get_Cloudflared_Url(verbose)
                    except Exception as E:
                        if verbose:
                            print(f"{self.color_important}[CFD]\tcloudflare free tunnel instability made phantom-domain with {E}... retrying {self.color_clear}")
                        subprocess.Popen(["killall", "cloudflared"])
                        await self._Get_Cloudflared_Url(verbose)
                    if verbose:
                        print(f"{self.color_important}[CFD]\tFOUND: {wss_url}{self.color_clear}")
                    self.url = wss_url
                    self.secret = await NetManager.DiscordManager.Server_Up(wss_url)
                    return
        async def Async_Close(self, url=None):
            await NetManager.DiscordManager.Close(url if url is not None else self.url)
            subprocess.Popen([
                "killall", "cloudflared"
                ])
            return 0;
        def _Close(self, url=None):
            asyncio.run(NetManager.DiscordManager(url if url is not None else self.url))
            subprocess.Popen(["killall", "cloudflared"])
            return 0
        def _Abort(self):
            self.close(self.url)
            exit(-1)

    def __init__(self):
        self.url = None
        self.secret = None
    async def _Start_Up(self, verbose):
        T = self.TunnelManager()
        await T._Get_Cloudflared_Url(verbose)
        self.url, self.secret = T.url, T.secret
        del(T)
        return self.url, self.secret
    async def _Close(self, verbose):
        # let clients finish up
        await asyncio.sleep(5)
        await self.DiscordManager.Close(self.url, verbose)
if __name__=="__main__":
    D = NetManager.DiscordManager("server")
    T = NetManager.TunnelManager()
    asyncio.run(T._Get_Cloudflared_Url(True))
    asyncio.run(NetManager.DiscordManager.Create_Client())
    asyncio.run(NetManager.DiscordManager.Close(T.url))
