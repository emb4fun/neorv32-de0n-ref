### README for neorv32-de0n-ref
It is not the reference but my reference. That means it includes the parts that I find important 
for my tests here. Others will certainly find other parts more important. But that is the nice 
thing about the NEORV32 project, here everyone can put their system together as they need it.

### Some notes about Mbed TLS
Mbed TLS is used in .\sw\crypto\library\mbedtls and was copied from the following project:
https://github.com/ARMmbed/mbedtls

#### Project information:

| Board    | [Terasic DE0-Nano](https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&CategoryNo=139&No=593) |
| :------- | :------------- |
| FPGA     | Cyclone IV `EP4CE22F17C6N` |
| Quartus  | 15.0.2         |
| clk_i    | 100 MHz        |
| Terminal | 19200, 8, N, 1 |

