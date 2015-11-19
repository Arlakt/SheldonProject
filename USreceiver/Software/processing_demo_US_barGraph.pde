// Feel Free to edit these variables ///////////////////////////
String xLabel = "Signal for each direction";
String yLabel = "Signal strength";
String Heading = "Received signals strength";
String URL = "14/11/2015";
float Vcc = 5.0; // the measured voltage of your usb
int NumOfVertDivisions=1; // dark gray
int NumOfVertSubDivisions=10; // light gray
int NumOfBars=8; // you can choose the number of bars, but it can cause issues
 // since you should change what the arduino sends
// if these are changed, background image has problems
// a plain background solves the problem
int ScreenWidth = 1024, ScreenHeight=720;
/////////////////////////////////////////////////////////
// Serial port stuff ///////////////////////
import processing.serial.*;
Serial myPort;
boolean firstContact = false;
int[] serialInArray = new int[8];
int serialCount = 0;
///////////////////////////////////////////////
int LeftMargin=100;
int RightMArgin=80;
int TextGap=50;
int GraphYposition=80;
float BarPercent = 0.4;
int value;
PFont font;
PImage bg;
int temp;
float yRatio = 0.58;
int BarGap, BarWidth, DivisounsWidth;
int[] bars = new int[NumOfBars];
void setup(){
/// NB SETTINGS ////////////////////////////////////////////////////////
printArray(Serial.list());
 myPort = new Serial(this, Serial.list()[1], 9600);
 ////////////////////////////////////////////////////////////////////////
DivisounsWidth = (ScreenWidth-LeftMargin-RightMArgin)/(NumOfBars);
 BarWidth = int(BarPercent*float(DivisounsWidth));
 BarGap = DivisounsWidth - BarWidth;
size(1024,720);
 font = createFont("Arial",12);
textAlign(CENTER);
 textFont(font);
}
void draw(){
// background(bg); // My one used a background image, I've
 background(250); // commented it out and put a plain colour
// Headings(); // Displays bar width, Bar gap or any variable.
 Axis();
 Labels();
 PrintBars();
}
// Send Recieve data //
void serialEvent(Serial myPort) {
// read a byte from the serial port:
 int inByte = myPort.read();

 // Add the latest byte from the serial port to array:
 serialInArray[serialCount] = inByte;
 serialCount++;
 if (serialCount >= 8 ) {
for (int x=0;x<8;x++){
bars[x] = int (yRatio*(ScreenHeight)*(serialInArray[x]/256.0));
}

 // Reset serialCount:
 serialCount = 0;
 }

}
/////// Display any variables for testing here//////////////
void Headings(){
 fill(0 );
 text("BarWidth",50,TextGap );
 text("BarGap",250,TextGap );
 text("DivisounsWidth",450,TextGap );
 text(BarWidth,100,TextGap );
 text(BarGap,300,TextGap );
 text(DivisounsWidth,520,TextGap );
}
void PrintBars(){
int c=0;
 for (int i=0;i<NumOfBars;i++){
fill((0xe4+c),(255-bars[i]+c),(0x1a+c));
 stroke(90);
 rect(i*DivisounsWidth+LeftMargin, ScreenHeight-GraphYposition, BarWidth, -bars[i]);
 fill(0x2e,0x2a,0x2a);
 text("S", i*DivisounsWidth+LeftMargin+BarWidth/2 -5, ScreenHeight-GraphYposition+20 );
 text(i+1, i*DivisounsWidth+LeftMargin+BarWidth/2 +5, ScreenHeight-GraphYposition+20 );
 }
}
void Axis(){
strokeWeight(1);
 stroke(90);
 line(LeftMargin-15, ScreenHeight-GraphYposition+2, ScreenWidth-RightMArgin-DivisounsWidth+50, ScreenHeight-GraphYposition+2);
 line(LeftMargin-15,ScreenHeight-GraphYposition+2,LeftMargin-15,GraphYposition);
 strokeWeight(1);
}
void Labels(){
 textFont(font,18);
 fill(50);
 rotate(radians(-90));
 text(yLabel,-ScreenHeight/2,LeftMargin-45);
 //textFont(font,16);
 //for(float x=0;x<=NumOfVertDivisions;x++){
 //int bars=(ScreenHeight-GraphYposition)-int(yRatio*(ScreenHeight)*(x/NumOfVertDivisions));
 //text(round(x),-bars,LeftMargin-20);
 //}
textFont(font,18);
 rotate(radians(90));
 text(xLabel,LeftMargin+(ScreenWidth-LeftMargin-RightMArgin-50)/2,ScreenHeight-GraphYposition+40);
 textFont(font,24);
 fill(50);
 text(Heading,LeftMargin+(ScreenWidth-LeftMargin-RightMArgin-50)/2,70);
 textFont(font);
fill(150);
 text(URL,ScreenWidth-RightMArgin-40,ScreenHeight-15);
 textFont(font);
}