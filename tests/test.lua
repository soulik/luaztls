local zmq = require 'zmq'
local ztls = require 'ztls'

local function test_https(host, url)
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
				local result = assert(s.recv(8192))
				print(result)
			end},
		}
		poll.start()
	end

	tlsClient.close()
end

test_https('google.com', '/')
