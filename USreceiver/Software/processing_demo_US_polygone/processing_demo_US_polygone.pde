
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
int startByte = 0;
int dataByteNumber = 1;
int signalNumber = 0;
long[] dataBeingConstructed = new long[8];


void setup(){
/// NB SETTINGS ////////////////////////////////////////////////////////
printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[1], 9600);
  ////////////////////////////////////////////////////////////////////////
  size(900,900);
  img = loadImage("drone.png");
  
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
  
  if(startByte == 1 && inByte != 0xFF)
    startByte = 0;
  else if(startByte == 0 && inByte == 0xFF)
    startByte = 1;
  else if(startByte == 1 && inByte == 0xFF)
  {
    startByte = 2;
    dataByteNumber = 1;
    signalNumber = 0;
  }
  else if(startByte == 2)
  {
    if(dataByteNumber == 1)
    {
      dataBeingConstructed[signalNumber] = 0x00 | (inByte << 8);
      dataByteNumber = 2;
    }
    else if(dataByteNumber == 2)
    {
      
      dataBeingConstructed[signalNumber] |= inByte;
      dataByteNumber = 1;
      signalNumber++;
    }
  }
  
  if(signalNumber >= 8)
  {
    for (int x=0;x<8;x++){
      SignalStrengthCorner[x] = (int)(dataBeingConstructed[x] * (long)maxRange / 50000); // should be 65536 but there is a problem and values don't go so high
    }
    signalNumber = 0;
    startByte = 0;
  }

}