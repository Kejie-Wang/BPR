'''
@brief This is the pre-processing procedure for the MF and BPR algorithms 
	   it just simplily selects the user, product and rating with each file with a specific number of users
	   one item occupies one line with the format (seprated by a blank): uid vid rating
	   		uid:	an integer represents the user id (1, 2, 3, ......)
	   		vid: 	an integer represents the product id (1, 2, 3, ......)
	   		rating: an float number represents the rating of the product of the user
@author WANG Kejie<wang_kejie@foxmail.com>
@date 	07/20/2016
'''

import json
import sys

print sys.argv

if len(sys.argv) < 2:
	print "Lack input parameters"
	exit()

MAXUSERNUM = 10000
user = {}
userNum = 0
for i in xrange(1, len(sys.argv)):
	path = sys.argv[i]
	cnt = 0
	with open(path) as fpRawData:
		for line in fpRawData:
			data = json.loads(line)
			uid_data = data["reviewerID"]
			if uid_data not in user:
				cnt += 1
				userNum += 1
				user[uid_data] = userNum
			if cnt >= MAXUSERNUM:
				break
	
	fpRawData.close()

print "total user number:", userNum


prefix = "amazon_mixture"
fileNameRating = prefix + "_rating.data"
fpRating = open(fileNameRating, 'w')

item = {}
itemNum = 0
for i in xrange(1, len(sys.argv)):
	path = sys.argv[i]
	suffix = ".json"
	if not path.endswith(suffix):
		print "the file name", path, "must end with .json.gz"
		continue
	with open(path) as fpRawData:
		for line in fpRawData:
			data = json.loads(line)
			uid_data = data["reviewerID"]
			vid_data = data["asin"]
			rating = data["overall"]
			if uid_data not in user:
				continue
			uid = user[uid_data]
			if vid_data in item:
				vid = item[vid_data]
			else:
				itemNum += 1
				vid = itemNum
				item[vid_data] = vid

			fpRating.write("%d %d %.01f\n" %(uid, vid, rating))

print "item number", itemNum

fpRating.close()
fpRawData.close()