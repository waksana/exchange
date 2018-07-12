compile: abi wast
abi:
	eosiocpp -g exchange.abi exchange.hpp
wast:
	eosiocpp -o exchange.wast exchange.cpp
clean:
	rm exchange.abi exchange.was*
