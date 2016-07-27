import random as ram
import sys

filename = sys.argv[1]

raw_data = filename+'_rating.data'

train_filename = filename+'.train'
validation_filename = filename+'.validation'
test_filename = filename+'.test'
stat_filename = filename+'.stat'

user_max_id = 0
item_max_id = 0

ftrain = open(train_filename, "w")
fvalidataion = open(validation_filename, "w")
ftest = open(test_filename, "w")

train_count = 0
validataion_count = 0
test_count = 0

with open(raw_data) as f:
	for line in f:
		data = line.split(' ')
		if int(data[0])>user_max_id:
			user_max_id = int(data[0])

		if int(data[1])>item_max_id:
			item_max_id = int(data[1])
		coin = ram.random()
		if coin < 0.8:
			if coin < 0.8 / 8:
				fvalidataion.write(line)
				validataion_count = validataion_count + 1
			else:
				ftrain.write(line)
				train_count = train_count + 1
		else:
			ftest.write(line)
			test_count = test_count + 1
f.close()
ftrain.close()
fvalidataion.close()
ftest.close()

user_max_id = user_max_id + 1
item_max_id = item_max_id + 1

f = open(stat_filename,'w')
line = str(user_max_id)+' '+str(item_max_id)+' '+str(train_count)+' '+str(validataion_count)+' '+str(test_count)
f.write(line)
f.close()
