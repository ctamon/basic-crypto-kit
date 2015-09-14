import sys

def usage():
	''' Print usage conventions '''
	print "Usage: python caesar.py <plaintext file> <shift amount>"


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


def main():
	if len(sys.argv) < 3:
		usage()
		return

	infile = open(sys.argv[1], 'r')
	shift = int(sys.argv[2])

	message = infile.read()
	cipher = caesar(message, shift)
	print cipher

	infile.close()


if __name__ == "__main__":
	main()




