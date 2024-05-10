import sys
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

def send_email():
    
    #sender_email = "visionvirtuoso09@gmail.com"
    sender_email = "healart61@gmail.com"

    receiver_email = sys.argv[1]
    subject = "Your Password"

    # Content from sys.argv[2] (verification code) 
    #verification_code = 123456
    verification_code = sys.argv[2]


    # Create message container - the correct MIME type is multipart/alternative.
    message = MIMEMultipart('alternative')
    message["Subject"] = subject
    message["From"] = sender_email
    message["To"] = receiver_email

    # Create the HTML message with CSS styling
    html = f"""
    <html>
      <head>
        <style>
          body {{
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            color: #202124;
            margin: 0;
            padding: 0;
          }}
          .container {{
            max-width: 600px;
            margin: 0 auto;
            padding: 20px;
            background-color: rgba(0, 0, 0, 0.1); /* Semi-transparent black background */
            border-radius: 10px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
          }}
          h1 {{
            color: #007bff;
          }}
          p {{
            line-height: 1.6;
          }}
          .verification-code {{
            background-color: #007bff;
            color: #fff;
            font-size: 24px;
            padding: 10px 20px;
            border-radius: 5px;
            display: inline-block;
            margin-bottom: 20px;
          }}
          .official-staff {{
            font-size: 14px;
            color: #777; /* Lighter color for official staff message */
          }}
        </style>
      </head>
      <body>
        <div class="container">
          <h1>Password</h1>
          <p>Dear User,</p>
          <p>Please find below your Password:</p>
          <p class="verification-code">{verification_code}</p>
          <p class="official-staff">Please do not share this with anyone. It is for your use only.</p>
          <p>Best regards,<br>Anazur</p>
        </div>
      </body>
    </html>
    """

    # Record the MIME types of both parts - text/plain and text/html.
    part1 = MIMEText(html, 'html')

    # Attach parts into message container.
    message.attach(part1)

    try:
        with smtplib.SMTP("smtp.gmail.com", 587) as server:
            server.starttls()
            #server.login(sender_email, "icrf davu ecwh tski")
            server.login(sender_email, "tazs quhm yqgu ryvd")
            
            server.sendmail(sender_email, receiver_email, message.as_string())
        print("Email sent successfully!")
    except Exception as e:
        print(f"An error occurred while sending the email: {e}")

# Call the function to send the email
send_email()
