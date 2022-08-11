# Networking demo AIS2203

Code showcasing data transmission using various networking technologies using C++.

#### Features:
* UDP
* TCP/IP
* ZMQ (Jack of all trades)
* Thrift (RPC framework)
* DDS (Publish-Subscribe)

Code is for demonstration purposes only, and is not necessary perfect.

By default, only UDP and TCP/IP demos are built. 
If you want to build other demos, set the corresponding CMake flag to `ON`:
```
-DWITH_THRIFT=ON
-DWITH_ZMQ=ON
-DWITH_DDS=ON
```

You can also pass `-DWITH_ALL=ON` if you want to build all the demos.

####  Requirements:
* CMake
* Conan
* Windows/Linux (MacOS might work, but has not been tested)

Conan can be installed using `pip`:
```bash
pip install conan
```
