'''Vigenere cipher'''

import sys

def usage():
	''' Print usage conventions '''
	print "Usage: python vigenere.py <plaintext-file> <passkey> <0 or 1>"


def char_shift(c, shift, base):
	pos = ord(c) - ord(base)
	pos = (pos + shift) % 26
	return chr(ord(base) + pos)


def caesar(plaintext, shift):
	cipher = ""
	for c in plaintext:
		if c.islower():
			c = char_shift(c, shift, 'a')
		elif c.isupper():
			c = char_shift(c, shift, 'A')
		cipher = cipher + c
	return cipher


def vigenere(plaintext, passkey, mode):
	pos = 0
	ciphertext = ""
	for index in range(len(plaintext)):
		if plaintext[index].isalpha():
			key_index = pos % len(passkey)
			shift = ord(passkey[key_index]) - ord('a')
			if mode == 1:
				shift = 26 - shift
			c = caesar(plaintext[index], shift)
			ciphertext = ciphertext + c
			pos = pos + 1
		else:
			ciphertext = ciphertext + plaintext[index]

	return ciphertext


def main():
	if len(sys.argv) < 4:
		usage()
		return

	infile = open(sys.argv[1], 'r')
	key = sys.argv[2]
	mode = int(sys.argv[3])

	message = infile.read()
	cipher = vigenere(message, key, mode)
	print cipher

	infile.close()


if __name__ == "__main__":
	main()




