#!/usr/bin/python

import sys
from sys import argv
from blake import * #import BLAKE hash function
import random
import string



def R(j, h): #reduction function, with parameter j to emulate R1, R2, ... , Rk
	print "reduced h"
	p = h
	return p


def H(p): #hash function
	print "hashed p"
	h = p 
	return h
	
	
def rainbowAttack(rt, hash, chain_len): #using the rainbow table rt, find the password corresponding to hash given
	print "performing rainbow attack"
	h = hash
	for key, value in rt.iteritems(): #check to see if the hash is an end_point, and if so return the start_point
		if (h == value): #found the hash, so get the corresponding key and recreate the chain until you find the hash again
			return key
				
	#if it wasn't in the table, start creating the chain. The same testing will be done after each chain iteration		
	#the chain I'm creating ,starting from the given hash, will start from Rk and go down to R1 as it searches the rainbow table for a match
	for j in xrange(0, chain_len): #until the end of the chain, reduce h and rehash it, then check if it's in the table
		p = R(chain_len - (j+1), h) 
		h = H(p)
		for key, value in rt.iteritems(): #test if current h is in the table
			if (h == value):
				pc = key
				for k in xrange(0, chain_len): #if it is, recreate the chain until you find h again or until you reach chain_len, starting from key
					hc = H(p) #hash the p password
					if(hc == h): #you found wanted hash, so return the password that produced it
						return pc
					pc = R(j, h) #reduce back to password
	return -1;


	

def createRainbowTable(rt, chain_num, chain_len):

	print "creating rainbow table"
	for i in xrange(0, chain_num): #create a starting point for eacg rt row, using a random string generator
		starting_point = ''.join([random.choice(string.ascii_letters + string.digits + '!@') for n in xrange(6)])
		p = starting_point
		for j in xrange(0, chain_len): #perform (hash function, redution function) "chain_len"" times to create each chain
			h = H(p) #hash the p password
			p = R(j, h) #reduce back to password
		end_point = h #the end_point to be stored in the table is the last hash of the chain, final p is omitted
		rt[starting_point] = end_point #store the chain
	print rt	
	

	

def main(argc, argv):

	if (argc < 2):
		print "Usage %s <hash>" %(argv[0])
		return
	chain_len = 10
	chain_num = 10
	rt = {} #empty rainbow table (dictionary)
	blake = BLAKE(256)
	createRainbowTable(rt, chain_num, chain_len)
	password = rainbowAttack(rt, argv[1], chain_len) #execute a rainbow attack using the rainbow table created on the first argument of the script(the hash)
	if(password == -1):
		print "Couldn't find the password"
	else:
		print "Password is %s" %(password)
	return
	

if __name__ == '__main__':
    main( len(argv), argv )