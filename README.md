<H1>Robot project</H1>
This is very old project of a rover that could follow blue ball. All code for LPC controller and related hardware was written by me over few months/years (I believe syarting back in 2008, all the way to 2014). I doubt code in this project could follow the blue ball - this was next step, more complex approach, but never got finished due to multiple reasons - main - time to dive into this.

One of the version of the platform in action:
https://vimeo.com/manage/videos/140847866

<h1>WHAT IS COOL ABOUT THIS PROJECT?</h1>
Collest (and most proud part of this one) is ethernet.c file ( https://github.com/dohtem81/robotProject_LPC/blob/master/robotProject/Common/source/ethernet.c )

It's very simple TCP stack that does send and receives TCP frames and responds to ping. It bases on ENC28J60.c ( https://github.com/dohtem81/robotProject_LPC/blob/master/robotProject/Common/source/ENC28J60.c ) which was direct driver to module hooked up to LPC23XX micropprocessor.

Let me know in case of questions pedziwiatr.piotr@gmail.com 
