# net-ttt
Network tic-tac-toe game in C with socket linux API.

## Overview

`net-ttt` is a simple client-server Tic-Tac-Toe game written in **C**, designed to run over a network using the **Linux socket API**.  
Two players can connect via TCP: one runs the **server**, the other runs the **client** and plays against it.

## Features

- Client-Server architecture using TCP sockets  
- Two-player gameplay over network  
- Turn-based game protocol with move validation  
- Game result logging on the server side  
- Simple terminal interface  

## Building

Use the included `Makefile`:

```bash
make all
