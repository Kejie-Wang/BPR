'''
@brief This is the parsing procedure for the Amazon dataset (http://jmcauley.ucsd.edu/data/amazon/)
	   which transforms the source .json.gz file into .json file
	   The input file path is need and the file name must end with .json.gz
	   e.g. python parse.py filename.json.gz
	   The output file is filename.json
@author WANG Kejie<wang_kejie@foxmail.com>
@date 	07/19/2016
'''
import json
import gzip
import sys

def parse(path):
	g = gzip.open(path, 'r')
	for l in g:
		yield json.dumps(eval(l))

if len(sys.argv) < 2:
	print "Lack input parameters"
	exit()

path = sys.argv[1]
suffix = ".json.gz"
if not path.endswith(suffix):
	print "the file name must end with .json.gz"
	exit()

prefix = path[0:-len(suffix)]

flieName = prefix + ".json"

f = open(flieName, 'w')
for l in parse(path):
	f.write(l + '\n')

user = {}
item = {}
userNum = 0
itemNum = 0

