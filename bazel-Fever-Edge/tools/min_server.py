#!/usr/bin/env python
"""
Very simple HTTP server in python (Updated for Python 3.7)

Usage:

    ./dummy-web-server.py -h
    ./dummy-web-server.py -l localhost -p 8000

Send a GET request:

    curl http://localhost:8000

Send a HEAD request:

    curl -I http://localhost:8000

Send a POST request:

    curl -d "foo=bar&bin=baz" http://localhost:8000

"""
import argparse
import base64
import binascii
import io
import json
import numpy as np
from PIL import Image
from fever_decipher import AESCipherUtils
from http.server import HTTPServer, BaseHTTPRequestHandler


key = "1550deb2825dd162"
iv = "d08c571d450df540"
cipher= AESCipherUtils(key, iv)

def RenderImage(payload):
  # prepare array
  picture_hex = base64.b64decode(payload)
  stream = io.BytesIO(picture_hex)
  img = Image.open(stream)
  print(img.size)
  img.save("/tmp/thermal_jpg.jpg", "JPEG")

class S(BaseHTTPRequestHandler):
    def _set_headers(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

    def _html(self, message):
        """This just generates an HTML document that includes `message`
        in the body. Override, or re-write this do do more interesting stuff.

        """
        content = "<html><body><h1>{}</h1></body></html>".format(message)
        return content.encode("utf8")  # NOTE: must return a bytes object!

    def do_GET(self):
        self._set_headers()
        self.wfile.write(self._html("hi!"))

    def do_HEAD(self):
        self._set_headers()

    def do_POST(self):
        # Doesn't do anything with posted data
        self._set_headers()
        self.wfile.write(self._html("POST!"))
        content_length = int(self.headers['Content-Length'])
        print(content_length)
        payload = self.rfile.read(content_length)
        payload = base64.b64decode(payload)
        print("Sample data: " + payload[:20])
        # print(payload[:40])
        payload = cipher.decrypt(payload)
        print(payload[:40])
        # payload = json.loads(payload)
        # RenderImage(payload["rgb_jpg_data"])

        # print("I received a POST data with length: " + str(len(payload))
        #     + ", machine_id: " + payload["machine_id"])



def run(server_class=HTTPServer, handler_class=S, addr="localhost", port=8000):
    server_address = (addr, port)
    httpd = server_class(server_address, handler_class)

    print("Starting httpd server on {}:{}".format(addr, port))
    httpd.serve_forever()


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Run a simple HTTP server")
    parser.add_argument(
        "-l",
        "--listen",
        default="localhost",
        help="Specify the IP address on which the server listens",
    )
    parser.add_argument(
        "-p",
        "--port",
        type=int,
        default=10021,
        help="Specify the port on which the server listens",
    )
    args = parser.parse_args()
    run(addr=args.listen, port=args.port)