#!/bin/sh
s-fuzz -d -i $AFLNET/tutorials/live555/in-rtsp -o out-live555 -Z ../../myfuzzer/yaml/live555.yaml ./testOnDemandRTSPServer 8554
