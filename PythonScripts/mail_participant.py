import sys
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

def send_email():
    sender_email = "visionvirtuoso09@gmail.com"
    receiver_email = sys.argv[1]
    subject = "Confirmation de participation"
    prenom = sys.argv[2]
    nomevenement = sys.argv[3]
    lieu = sys.argv[4]
    description = sys.argv[5]
    date_debut = sys.argv[6]
    date_fin = sys.argv[7]
    
    # Code HTML modifié avec les informations fournies
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
          .official-staff {{
            font-size: 14px;
            color: #777; /* Lighter color for official staff message */
          }}
        </style>
      </head>
      <body>
        <div class="container">
          <h1>Confirmation de participation</h1>
          <p>Cher {prenom},</p>
          <p>Vous venez de vous inscrire chez VIRTUOSO VISION pour leur événement {nomevenement} qui se situe à {lieu} du {date_debut} au {date_fin}.</p>
          <p>Description :{description}</p>
          <p class="official-staff">Veuillez ne pas hésiter à nous contacter si vous avez des questions ou des préoccupations.</p>
          <p>Cordialement,<br>VIRTUOSO VISION</p>
        </div>
      </body>
    </html>
    """

    # Create message container - the correct MIME type is multipart/alternative.
    message = MIMEMultipart('alternative')
    message["Subject"] = subject
    message["From"] = sender_email
    message["To"] = receiver_email

    # Record the MIME types of both parts - text/plain and text/html.
    part1 = MIMEText(html, 'html')

    # Attach parts into message container.
    message.attach(part1)

    try:
        with smtplib.SMTP("smtp.gmail.com", 587) as server:
            server.starttls()
            server.login(sender_email, "icrf davu ecwh tski")
            server.sendmail(sender_email, receiver_email, message.as_string())
        print("Email sent successfully!")
    except Exception as e:
        print(f"An error occurred while sending the email: {e}")

# Call the function to send the email
send_email()
