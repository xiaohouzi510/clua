so = require "luaso"
hello = {}
function hello:test(str)
	print(str)
end

user_data = so.create(hello)
user_data:call_back()

function hook_tow(event)
	local info = debug.getinfo(2,"nSl")	
	print(event,info.name,info.currentline,info.short_src)
	return false
end

function hook_test(event)
	return hook_tow(event)	
end

function one(str)
	if str then
		print(str)
	end
end

function tow(s)
	if s then
		one(s)
	end
end

function co_test(str)
	print("------------",str)
	so.set_hook(hook_test,"crl")
	one("hello")
	tow("world")
end

co = coroutine.create(co_test)
coroutine.resume(co,"start")
print(coroutine.status(co))
