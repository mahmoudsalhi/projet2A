import speech_recognition as sr

recognizer = sr.Recognizer()

with sr.Microphone() as source:
    recognizer.adjust_for_ambient_noise(source)  # Adjust for ambient noise
    try:
        audio = recognizer.listen(source, timeout=2)  # Record audio with a 10-second timeout
    except sr.WaitTimeoutError:
        print("No speech detected within the timeout period.")
        exit()

try:
    text = recognizer.recognize_google(audio)
    print(text)
except sr.UnknownValueError:
    print("Sorry, could not understand audio.")
except sr.RequestError as e:
    print("Could not request results; {0}".format(e))
