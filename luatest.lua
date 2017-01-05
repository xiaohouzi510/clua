so = require "luaso"
hello = {}
function hello:test(str)
	print(str)
end

user_data = so.create(hello)
user_data:call_back()
