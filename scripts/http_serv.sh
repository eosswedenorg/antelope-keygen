#!/usr/bin/env python
import os, BaseHTTPServer, SimpleHTTPServer

port=8000
print "Running on port %d" % port

base_path = os.path.dirname(os.path.realpath(__file__))
os.chdir(base_path + "/../build")

SimpleHTTPServer.SimpleHTTPRequestHandler.extensions_map['.wasm'] =    'application/wasm'
httpd = BaseHTTPServer.HTTPServer(('localhost', port),
    SimpleHTTPServer.SimpleHTTPRequestHandler)

print "Mapping \".wasm\" to \"%s\"" % SimpleHTTPServer.SimpleHTTPRequestHandler.extensions_map['.wasm']
httpd.serve_forever()
