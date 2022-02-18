import numpy as np
import cv2, os, sys
face_cascade = cv2.CascadeClassifier('../../opencv_build/opencv/data/haarcascades/haarcascade_frontalface_default.xml')

# All the faces data will be
#  present this folder
datasets = 'datasets' 
 
 
# These are sub data sets of folder,
# for my faces I've used my name you can
# change the label here
sub_data = sys.argv[1]  
 
path = os.path.join(datasets)
if not os.path.isdir(path):
    os.mkdir(path)

cam = cv2.VideoCapture(0)

cv2.namedWindow("test")

img_counter = 0

while True:
    ret, frame = cam.read()
     # to detect faces in video
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    for (x,y,w,h) in faces:
	    cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),2)
	    roi_gray = gray[y:y+h, x:x+w]
	    roi_color = frame[y:y+h, x:x+w]

    x = 0
    y = 20
    text_color = (0,255,0)
    if not ret:
        print("failed to grab frame")
        break
    cv2.imshow("test", frame)

    k = cv2.waitKey(1)
    if k%256 == 27:
        # ESC pressed
        print("Escape hit, closing...")
        break
    elif k%256 == 32:
        # SPACE pressed
        img_name = "%s.jpeg"%sub_data
        cv2.imwrite(img_name, frame)
        print("{} written!".format(img_name))
 #       img_counter += 1

cam.release()

cv2.destroyAllWindows()
