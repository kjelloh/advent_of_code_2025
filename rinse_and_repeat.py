#!/usr/bin/env python3
import time, os, subprocess

CMD_FILE = "command_to_repeat.txt"
INTERVAL = 5

# Ensure the file exists
open(CMD_FILE, 'a').close()

while True:
    with open(CMD_FILE, 'r') as f:
        cmd = f.read().strip()
    
        os.system("clear")
        print(f"Running: {cmd}")
        subprocess.run(cmd, shell=True)

    time.sleep(INTERVAL)
