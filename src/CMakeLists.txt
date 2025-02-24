set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(WNET STATIC
			Subsystem.cpp
			Socket.cpp
			UdpSocket.cpp
			PollFD.cpp
			TcpSocket.cpp
			)
