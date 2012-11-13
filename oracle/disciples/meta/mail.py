# -*- encoding: utf-8 -*-

from meta import conf

from email.mime.text import MIMEText
from email.header import Header
#from email.utils import parseaddr, formataddr
import smtplib

def send(mail, subject, content,
         from_="Infinit <no-reply@infinit.io>",
         encoding='utf8'):
        msg = MIMEText(content, _charset=encoding)
        msg['Subject'] = Header(subject, encoding)
        msg['From'] = Header(from_, encoding)
        msg['To'] = Header(mail, encoding)

        smtp_server = smtplib.SMTP(conf.MANDRILL_SMTP_HOST, conf.MANDRILL_SMTP_PORT)
        try:
            smtp_server.login(conf.MANDRILL_USERNAME, conf.MANDRILL_PASSWORD)
            smtp_server.sendmail(msg['From'], [msg['To']], msg.as_string())
        finally:
            smtp_server.quit()


#################################################################################
# Mail subjects and body
#################################################################################

# XXX: use mailchimp templates

# Admin invitation:
INVITATION_SUBJECT = "Invitation to test Infinit!"
INVITATION_CONTENT = """
Dear Alpha tester,

Welcome to Infinit! You are one of the first individuals to get to try it out!

Remember that the version you will be using is still quite basic and only
functions in a local network at the moment. It may still have bugs and the
interface is still not in its public version. For now, file sharing is limited
to 30 MB and only the network owner can edit the files. Don’t worry though!
We’ll be updating all of this over the coming weeks.

You can report bugs or simply tell us what you think by sending us an email at
feedback@infinit.io or by visiting feedback.infinit.io. If you have any
questions, you can reach us on Skype at infinitdotio.

To get started, download Infinit here: http://infinit.io/download.

Your access code is: %(activation_code)s

If there are other people with whom you would like to use Infinit, please let
us know by sending an email to community@infinit.io.

Instructions:

 * MacOSX (only Mountain Lion):
    1) Install Infinit and create an account with your access code.
    2) Create a network.
    3) Drag and drop other users into your network.
    4) Drag and drop files into your network giving everyone read-only access
       to them.

 * Linux:
    1) Register a new account and start the Infinit shell:
        $ infinit register shell
    2) Start Infinit instances (in the Infinit shell):
        infinit-shell$ start
    3) Create a new network:
        infinit-shell$ create_network MyNetwork
    4) You might need to refresh networks with:
        infinit-shell$ refresh_networks
    5) Invite a friend to a network:
        infinit-shell$ invite my@friend.com MyNetwork

All the best,

--%(space)s
The Infinit Team
http://infinit.io
""".strip()


#user infitation
USER_INVITATION_SUBJECT = "%(inviter_mail)s wants to share a file through Infinit!"
USER_NEW_FILE_CONTENT = """
Dear user,

%(inviter_mail)s wants to share %(file_name)s.

--
The Infinit Team
http://infinit.io
""".strip()
USER_INVITATION_CONTENT = """
Dear user,

%(inviter_mail)s wants to share %(file_name)s and make you discover Infinit.

XXX
blabla, not stable, blabla, alpha, blabla, danger, blabla, destroy all your data, blabla.

To get started, download Infinit here: http://infinit.io/download.

IMPORTANT:
XXX
blabla, use this email, blabla

If there are other people with whom you would like to use Infinit, please let
us know by sending an email to community@infinit.io.

Instructions:

 * MacOSX (only Mountain Lion):
    1) Install Infinit and create an account with your access code.
    2) Roll a d100
    3) Add 50 cause you are a mac user
    4) Refer to table below to discover your anus breaking point.

| Result* >>>> Size
-----------------------------------------
| 51 - 55 >>>> IPod shuffle
| 56 - 60 >>>> IPod nano
| 61 - 65 >>>> IPod Classic
| 66 - 75 >>>> IPod Touch | IPhone 1,2,3
| 76 - 80 >>>> IPhone 4-5
| 81 - 85 >>>> IPad Mini
| 86 - 95 >>>> IPad
| 96 - 105 >>> Mac book mini
| 106 - 115 >> Mac book 13
| 116 - 125 >> Mac book 15
| 126 - 135 >> iMac 21
| 136 - 145 >> iMac 27
| 145 - 150 >> eMac

(*) Add 5 for each apple product you already purchased !

 * Linux:
    1) Wait for infinit to be integrated.

All the best,

--
The Infinit Team
http://infinit.io
""".strip()
