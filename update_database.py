#!/usr/bin/python3

import json
import os

pwd = os.getcwd()

with open('compile_commands.json', 'r') as f:
    database = json.loads(f.read())

for entry in database:
    entry['directory'] = pwd


with open('compile_commands.json', 'w') as f:
    f.write(json.dumps(database))

print('Done!')
