#!/bin/bash
PORT=8080
PID=$(lsof -t -i:$PORT)
if [ ! -z "$PID" ]; then
	echo "Killing process $PID using port $PORT"
	kill -9 $PID
fi
