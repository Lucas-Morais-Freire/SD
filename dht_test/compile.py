import subprocess as sp

sp.call('arduino-cli compile dht_test.ino --fqbn=arduino:avr:mega --build-path=build')