
int ScreenWidth = 900, ScreenHeight=900;
int xCenter = ScreenWidth/2;
int yCenter = ScreenHeight/2;
int maxRange = 225;
int rangeOffset = 125;
int[] SignalStrengthCorner = new int[8];
PImage img;

/////////////////////////////////////////////////////////
// Serial port stuff ///////////////////////
import processing.serial.*;
Serial myPort;
int[] serialInArray = new int[8];
int serialCount = 0;


void setup(){
/// NB SETTINGS ////////////////////////////////////////////////////////
printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[1], 9600);
  ////////////////////////////////////////////////////////////////////////
  size(900,900);
  img = loadImage("drone.png");
  
  // Start analog read
  myPort.write('S');
  myPort.write('S');
  myPort.write('S');
  myPort.write('S');
  myPort.write('S');
}


void draw(){
  background(250);
  fill(180);
  stroke(255);
  strokeWeight(2);
  beginShape();
  // S1
  vertex(xCenter, yCenter-(SignalStrengthCorner[0]+rangeOffset));
  // S2
  vertex(xCenter-0.707*(SignalStrengthCorner[1]+rangeOffset), yCenter-0.707*(SignalStrengthCorner[1]+rangeOffset));
  // S3
  vertex(xCenter-(SignalStrengthCorner[2]+rangeOffset), yCenter);
  // S4
  vertex(xCenter-0.707*(SignalStrengthCorner[3]+rangeOffset), yCenter+0.707*(SignalStrengthCorner[3]+rangeOffset));
  // S5
  vertex(xCenter, yCenter+(SignalStrengthCorner[4]+rangeOffset));
  // S6
  vertex(xCenter+0.707*(SignalStrengthCorner[5]+rangeOffset), yCenter+0.707*(SignalStrengthCorner[5]+rangeOffset));
  // S7
  vertex(xCenter+SignalStrengthCorner[6]+rangeOffset, yCenter);
  // S8
  vertex(xCenter+0.707*(SignalStrengthCorner[7]+rangeOffset), yCenter-0.707*(SignalStrengthCorner[7]+rangeOffset));
  endShape();
  image(img, xCenter-100, yCenter-100, 200, 200);
}


// Recieve data //
void serialEvent(Serial myPort) {
  // read a byte from the serial port:
   int inByte = myPort.read();
  
   // Add the latest byte from the serial port to array:
   serialInArray[serialCount] = inByte;
   serialCount++;
   if (serialCount >= 8 ) {
   for (int x=0;x<8;x++){
      SignalStrengthCorner[x] = serialInArray[x] * maxRange / 256;
    }
  
   // Reset serialCount:
   serialCount = 0;
   }

}