import processing.serial.*;

Serial myPort; 
String val;    
int[] data; 

 void plotdata()
  {

    background(0); 
    stroke(255); 
    for(int i=0; i<data.length-1; i++)
    {
      line(i, 0, i,512-data[i]);
    }
  }


void setup() 
{
  println(Serial.list());
  String portName = Serial.list()[2]; //change # to match your port
  myPort = new Serial(this, portName, 9600);
  
  size(255,512);
}

void draw()
{
  if ( myPort.available() > 0) 
  {  
    val = myPort.readStringUntil('\n');         // read it and store it in val
    if (val != null)
    {
      data = int(split(val, ',')); 

      for (int i = 0; i < data.length; i++) 
      {
        print(data[i]);
        print(' ');
      }
      println( ' ');
      plotdata();
    }
  }
}

