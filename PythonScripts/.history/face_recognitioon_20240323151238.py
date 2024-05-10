import os
import face_recognition
import cv2
import requests
import numpy as np

# Path to the directory containing face images
faces_directory = r"C:\Users\bechi\Desktop\ProjetC++\PythonScripts\faces"

# Load known face encodings
known_face_encodings = []
known_face_names = []

# Iterate over the face images in the directory
for filename in os.listdir(faces_directory):
    if filename.endswith(".jpg") or filename.endswith(".png"):
        # Load the image and encode the face
        image_path = os.path.join(faces_directory, filename)
        face_image = face_recognition.load_image_file(image_path)
        face_encoding = face_recognition.face_encodings(face_image)[0] 

        # Extract the name from the filename
        name = os.path.splitext(filename)[0]

        # Add the face encoding and name to the known face encodings
        known_face_encodings.append(face_encoding)
        known_face_names.append(name)

# URL to the DroidCam video stream
droidcam_url = "http://192.168.1.2:4747/video"

# Open the video stream
stream = requests.get(droidcam_url, stream=True)
bytes = bytes()
nbNotmatch=0
# Iterate over the frames from the video stream
for chunk in stream.iter_content(chunk_size=1024):
    bytes += chunk
    a = bytes.find(b'\xff\xd8')
    b = bytes.find(b'\xff\xd9')
    if a != -1 and b != -1:
        jpg = bytes[a:b + 2]
        bytes = bytes[b + 2:]

        #frame = cv2.imdecode(np.fromstring(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)

        frame = cv2.imdecode(np.frombuffer(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)


        # Convert the frame to RGB
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Find all the faces and their encodings in the frame
        face_locations = face_recognition.face_locations(rgb_frame)
        face_encodings = face_recognition.face_encodings(rgb_frame, face_locations)
        match = False
        
        # Iterate over the face encodings in the frame
        for face_encoding in face_encodings:
            # Compare the face encoding with the known face encodings
            matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
            name = "Unknown"
            nbNotmatch+=1

            # Find the best match
            if True in matches:
                index = matches.index(True)
                name = known_face_names[index]
                match = True
                break      

        if match:
            # Print the name of the recognized face
            print(f"{name}")   
            break
        else:
            if nbNotmatch>4:
                print("Unkown")
                break
        # Display the frame
        cv2.imshow('Frame', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

# Release the video stream and close OpenCV windows
cv2.destroyAllWindows()
