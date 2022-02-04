import base64
import hashlib
from Crypto import Random
from Crypto.Cipher import AES

class AESCipherUtils(object):
  def __init__(self, key, iv):
      self.bs = AES.block_size
      self.key = key
      self.iv = iv

  def encrypt(self, raw):
      raw = self._pad(raw)
      cipher = AES.new(self.key, AES.MODE_CFB, self.iv)
      return base64.b64encode(iv + cipher.encrypt(raw.encode()))

  def decrypt(self, enc):
      cipher = AES.new(self.key, AES.MODE_CFB, self.iv)
      print(self.key)
      return cipher.decrypt(self.iv + enc)

  def _pad(self, s):
      return s + (self.bs - len(s) % self.bs) * chr(self.bs - len(s) % self.bs)

  @staticmethod
  def _unpad(s):
      return s[:-ord(s[len(s)-1:])]