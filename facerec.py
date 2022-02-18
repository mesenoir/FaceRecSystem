#python3 facerec.py

from datetime import datetime
import cv2
import numpy as np
import face_recognition
import sys

today  = datetime.today()
date_time = today.strftime("%m.%d.%Y")
file_object  = open(date_time+".txt", "a+")

now = datetime.now()
current_time = now.strftime("%H:%M:%S")

imgOrig = face_recognition.load_image_file(sys.argv[1]+".jpeg")
imgOrig = cv2.cvtColor(imgOrig,cv2.COLOR_BGR2RGB)
imgTest = face_recognition.load_image_file("client.jpeg")
imgTest = cv2.cvtColor(imgTest,cv2.COLOR_BGR2RGB)

faceLoc = face_recognition.face_locations(imgOrig)[0]
encodeOrig = face_recognition.face_encodings(imgOrig)[0]
cv2.rectangle(imgOrig,(faceLoc[3],faceLoc[0]),(faceLoc[1],faceLoc[2]),(255,0,255),2)

faceLocTest = face_recognition.face_locations(imgTest)[0]
encodeTest = face_recognition.face_encodings(imgTest)[0]
cv2.rectangle(imgTest,(faceLocTest[3],faceLocTest[0]),(faceLocTest[1],faceLocTest[2]),(255,0,255),2)

results = face_recognition.compare_faces([encodeOrig],encodeTest)
print(results)

if results==[True]:
	file_object.write(sys.argv[1] + "\t" + current_time +"\n")

file_object.close()

#cv2.imshow('Database Photo', imgOrig)
#cv2.imshow('Webcam Photo',imgTest)
cv2.waitKey(0)
