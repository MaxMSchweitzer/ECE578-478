import socket
import sys

def main():
	stand_page_number = 132
	command_sent = 0
	#posture_file = open("../cat_robot_posture.txt", "r")
	robot_info = {}
	command_list = {}
	# Open jimmy info file and create dictionary
	# Currently this project only uses one robot
	with open("jimmy_client_info.txt","r") as fin:
		for line in fin:
			line = line.strip('\n')
			parts = line.split(',')
			temp = {}
			temp['host'] = parts[1]
			temp['port'] = int(parts[2])
			robot_info[parts[0]] = temp
	
	print robot_info
	previous_data = 1
	
	response = "a"

	# cat_robot_posture.txt is the output file of the OpenCV C++ program, it will
	# either have a 0 or 1 in it, for id the robot is laying down or standing respectively
	while response != "q":
		posture_file = open("../cat_robot_posture.txt", "r")
		data = posture_file.readline()
		posture_file.close()

		try:
			if int(data) == 1:
				command_sent = 0
				previous_data = data
		except:
			pass
		
		host = temp['host']
		HOST = '{}'.format(host)
		PORT = temp['port'] 
		
		try:
			# Try to open a UDP connection, and send the command to the robot
			if int(command_sent) == 0 and int(data) == 0:
				sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
				# 3 is the command used by Motion script to play a page number from RME,
				# this could be extended to send other kinds of commands as well, such as walk
				# or turn. Assign stand_page_number above for what page number you want to send over
				command = "3 %d" % (stand_page_number)
				sock.sendto(command, (HOST, PORT))
				print HOST
				print PORT
				print "Sent:     %s" % (command)
				command_sent = 1
		except:
			pass


if __name__ == "__main__":
	main()
