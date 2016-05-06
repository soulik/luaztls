local zmq = require 'zmq'
local ztls = require 'luaztls'

local M = {}

M.client = function(a,b,c)
	local context, zmq_context, data_endpoint, control_endpoint

	if type(a)=='string' then
		data_endpoint, control_endpoint = a,b
		context = ztls.clientNew(data_endpoint, control_endpoint)
	elseif type(a)=='userdata' then
		zmq_context, data_endpoint, control_endpoint = a,b,c
		context = ztls.clientNew(zmq_context, data_endpoint, control_endpoint)
	end

	assert(context, "Can't create ZTLS Context")
	
	local lfn = {
		connect = function(hostname, port)
			assert(ztls.clientConnect(context, hostname, port))
		end,
		CAChain = function(CAChain)
			assert(ztls.clientCAChain(context, CAChain))
		end,
		close = function()
			assert(ztls.clientClose(context))
		end,
	}

	local mt = getmetatable(context)
	mt.__index = function(t, fn)
		return lfn[fn]
	end
	mt.__gc = function()
		assert(ztls.clientDestroy(context))
	end
	return context

end


return M
