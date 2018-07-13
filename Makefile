deploy: compile
	cleos set contract exchange dest/
compile: abi wast
abi: dest
	eosiocpp -g dest/dest.abi src/exchange.hpp
wast: dest
	eosiocpp -o dest/dest.wast src/exchange.cpp
clean:
	rm -rf dest
dest:
	mkdir dest
