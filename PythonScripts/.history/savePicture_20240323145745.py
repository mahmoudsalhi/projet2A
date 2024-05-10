import cv2
import os
import sys

def get_unique_filename(save_path):
    # Extract directory and base filename
    directory, filename = os.path.split(save_path)
    filename_no_ext, ext = os.path.splitext(filename)

    # Check if the file already exists
    if os.path.exists(save_path):
        # If the file exists, find a unique filename
        i = 1
        while True:
            new_filename = f"{filename_no_ext}_{i}{ext}"
            new_save_path = os.path.join(directory, new_filename)
            if not os.path.exists(new_save_path):
                return new_save_path
            i += 1
    else:
        # If the file doesn't exist, return the original save path
        return save_path

def capture_and_save_image(save_path):
    droidcam_url = "http://192.168.1.2:4747/video"
    camera = cv2.VideoCapture(droidcam_url)

    # Check if the camera is opened successfully
    if not camera.isOpened():
        print("Error: Unable to access the camera.")
        return

    while True:
        # Capture a frame
        ret, frame = camera.read()
        frame_without_message=
        message = "Press SPACE to take a picture."
        cv2.putText(frame, message, (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

        # Display the frame
        cv2.imshow("Camera", frame)

       

        # Check for key press
        key = cv2.waitKey(1)
        if key == ord(' '):  # If SPACE is pressed
            cv2.putText(frame, "         rgeqrgergrg", (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 0), 2)
            # Get a unique filename
            unique_save_path = get_unique_filename(save_path)

            # Save the captured image
            cv2.imwrite(unique_save_path, frame)
            print(f"Image saved successfully at: {unique_save_path}")
            break

    # Release the camera and close OpenCV windows
    camera.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    save_path = "C:\\Users\\bechi\\Desktop\\ProjetC++\\PythonScripts\\faces\\" + sys.argv[1] + ".jpg"
    capture_and_save_image(save_path)
