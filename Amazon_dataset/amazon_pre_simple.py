'''
@brief This is the pre-processing procedure for the MF and BPR algorithms 
	   it just simplily selects the user, product and rating
	   one item occupies one line with the format (seprated by a blank): uid vid rating
	   		uid:	an integer represents the user id (1, 2, 3, ......)
	   		vid: 	an integer represents the product id (1, 2, 3, ......)
	   		rating: an float number represents the rating of the product of the user
@author WANG Kejie<wang_kejie@foxmail.com>
@date 	07/20/2016
'''

import json
import sys

if len(sys.argv) < 2:
	print "Lack input parameters"
	exit()

path = sys.argv[1]
suffix = ".json"
if not path.endswith(suffix):
	print "the file name must end with .json.gz"
	exit()

prefix = "amazon_simple_" + path[0:-len(suffix)] 
fileNameRating = prefix + "_rating.data"

fpRating = open(fileNameRating, 'w')

user = {}
item = {}
userNum = 0
itemNum = 0

with open(path) as fpRawData:
	for line in fpRawData:
		data = json.loads(line)
		uid_data = data["reviewerID"]
		vid_data = data["asin"]
		rating = data["overall"]
		if uid_data in user:
			uid = user[uid_data]
		else:
			userNum += 1
			uid = userNum
			user[uid_data] = uid
		if vid_data in item:
			vid = item[vid_data]
		else:
			itemNum += 1
			vid = itemNum
			item[vid_data] = vid

		fpRating.write("%d %d %.01f\n" %(uid, vid, rating))

fpRating.close()
fpRawData.close()