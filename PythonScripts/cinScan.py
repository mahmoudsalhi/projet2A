
import cv2
import pytesseract

# Path to the Tesseract executable (change it if necessary)
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'

def extract_numbers_from_camera():
    # URL to connect to DroidCam
    #droidcam_url = f"http://192.168.1.2:4747/video"
    # Initialize DroidCam capture
    #droidcam = cv2.VideoCapture(droidcam_url)
    video_capture = cv2.VideoCapture(0)
    
    while True:
        # Capture frame from camera
        ret, frame = video_capture.read()

        if not ret:
            print("Error: Unable to capture frame")
            break

        # Convert frame to grayscale
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Use pytesseract to extract text from the frame
        extracted_text = pytesseract.image_to_string(gray_frame)

        # Extract numbers from the extracted text
        numbers = [int(num) for num in extracted_text.split() if num.isdigit() and len(num) == 8]

        # Display the frame with extracted numbers
        cv2.imshow("Camera Feed", frame)
        if numbers:
            print(numbers[0])
            break

        # Break the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the camera capture
    video_capture.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    extract_numbers_from_camera()
