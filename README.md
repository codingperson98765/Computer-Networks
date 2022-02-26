# Clown Proxy Server

Created a web proxy in C that intercepts HTTP web requests from browser and alters content items before they are delivered back to the browser.
It alters two components:
1. All jpgs on a webpage are replaced with an image of a happy clown instead
2. Any occurence of the work "Happy" is changed to the word silly.

Proxy handles GET requests, and uses TCP socket programming in C to handle client-proxy communication. 
