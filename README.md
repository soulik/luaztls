# luaztls
Lua binding for ZeroMQ TLS proxy with mbedtls library

This project allows you to interconnect ZeroMQ network stack with SSL/TLS enabled services e.g.: https, imaps, pop3s, ...

## Requirements

You'll need to use ZeroMQ PAIR socket type to send/receive data over TLS.

See my other project LuaZMQ at https://github.com/soulik/luazmq

## Usage

You should include following line somewhere in your Lua code:

```lua
local ztls = require 'ztls'
```

> Be sure to read incomming data in large enough chunks or you might lose some data. It's usually good to use chunks with size of 8KB.

## API

* Initialization

  Initializes and returns ZTLS context object.

  * **zeromq_ctx** - ZeroMQ context object (needed only for IPC/Inproc transport)
  * **data_endpoint** - used to transport unencrypted communication data
  * **control_endpoint** - used for debugging purposes, may contain info/error messages (not mandatory parameter)

  ``ztls_ctx = ztls.client(data_endpoint, control_endpoint)``

  ``ztls_ctx = ztls.client(zeromq_ctx, data_endpoint, control_endpoint)``

* Connection

  Connects to remote server and initializes TLS connection.
  
  * **host** - Remote hostname/IPv4/IPv6
  * **port** - Port number
  
  ``ztls_ctx.connect(host, port)``
  
  ``ztls_ctx.close()``

## Examples

Following example connects to well known website and grabs its content.

```lua
local zmq = require 'zmq'
local ztls = require 'ztls'

local function test_https(host, url)
    local CHUNK_SIZE = 8192
	local data_endpoint = 'inproc://https'
	local ctx = zmq.context()
	local tlsClient = assert(ztls.client(ctx, data_endpoint))

	local s = assert(ctx.socket(zmq.ZMQ_PAIR))

	assert(s.connect(data_endpoint))

	local request = ("GET %s HTTP/1.1\r\nHost: %s\r\n\r\n"):format(url, host)

	tlsClient.connect(host, 443)

	local len = assert(s.send(request))
	if len and len > 0 then
		local poll = zmq.poll {
			{s, zmq.ZMQ_POLLIN, function(s)
				local result = assert(s.recv(CHUNK_SIZE))
				print(result)
			end},
		}
		poll.start()
	end

	tlsClient.close()
end

test_https('google.com', '/')
```


## Authors
* Mário Kašuba <soulik42@gmail.com>

## Copying
Copyright 2016 Mário Kašuba
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
