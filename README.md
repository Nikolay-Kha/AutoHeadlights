Simple project to implement universal DRL(daytime running lights) and automatic headlights controller using simple MODBUS-RTU 4 way relay module with STM8 MCU which was bought [here](https://www.aliexpress.com/item/MODBUS-RTU-4-way-relay-module-STM8S103-MCU-two-development-RS485-communication-TTL/32870342824.html). Descripton says about STM8S103, but I got STM8S003, it doesn't matter for our purposes. That board has SWIM interface to reprogram board and use for any other purposes. Presence of STM8 MCU, 12V power input and relays makes this board perfect for automotive use.

# Board MCU connections
S1 switch - reset mcu  
S2 switch - PA3  
LED - PD4  
IN1 - PD3  
IN2 - PD2  
IN3 - PC7  
IN4 - PC6  
Relay 1 (low is on) - PB4  
Relay 2 (low is on) - PC3  
Relay 3 (low is on) - PC4  
Relay 4 (low is on) - PC5  
UART TX - PD5  
UART RX - PD6  

# Dependecies
```
brew install sdcc

git clone https://github.com/vdudouyt/stm8flash.git
cd stm8flash
make
sudo make install
```

# Chip
STM8S003F3P6  
Unlock chip:
```
stm8flash -c stlinkv2 -p stm8s003?3 -u
```

# Hardware mod
There is a need to add resstors and protective diods to board input.  
![](hwmod.jpg?raw=true) 

# Development connection to STLink v2
![](board.jpg?raw=true) 

# License
`sdk` project directory is a part of official SDK for microcontroller, see files inside for license information  
See LICENSE file for everything else.
