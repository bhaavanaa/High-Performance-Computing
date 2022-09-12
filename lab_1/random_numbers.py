import random
rand=[]
for i in range(500000):
    rand.append(random.uniform(10,10000000))

file = open("random1.txt", "w")
for i in rand:
	file.write(str(i))
	file.write("\n")
file.close()