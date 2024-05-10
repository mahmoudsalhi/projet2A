import cv2

def capture_and_save_image(save_path):
    # Initialize the camera
    camera = cv2.VideoCapture(0)

    # Check if the camera is opened successfully
    if not camera.isOpened():
        print("Error: Unable to access the camera.")
        return

    print("Press SPACE to take a picture.")

    while True:
        # Capture a frame
        ret, frame = camera.read()

        # Display the frame
        cv2.imshow("Press SPACE to take picture", frame)

        # Check for key press
        key = cv2.waitKey(1)
        if key == ord(' '):  # If SPACE is pressed
            # Save the captured image
            cv2.imwrite(save_path, frame)
            print(f"Image saved successfully at: {save_path}")
            break

    # Release the camera and close OpenCV windows
    camera.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    save_path = "captured_image.jpg"  # Change the path as needed
    capture_and_save_image(save_path)
