/*
41128 DRAM playground based on 4164 DRAM playground
*/

#define RAS1        3
#define RAS2        4
#define CAS         5
#define D           6 // chip data IN
#define Q           8 // chip data OUT
#define WRITE       9
#define STATUS_LED  7

// the setup function runs once when you press reset or power the board
void setup() 
{
  Serial.begin(9600);
  
  pinMode(A0, OUTPUT);  
  pinMode(A1, OUTPUT);  
  pinMode(A2, OUTPUT);  
  pinMode(A3, OUTPUT);  
  pinMode(A4, OUTPUT);  
  pinMode(A5, OUTPUT);  
  pinMode(10, OUTPUT);  
  pinMode(11, OUTPUT);  

  pinMode(D, OUTPUT); 
  pinMode(Q, INPUT); 

  pinMode(RAS1, OUTPUT); 
  pinMode(RAS2, OUTPUT);
  pinMode(CAS, OUTPUT); 
  pinMode(WRITE, OUTPUT); 

  pinMode(STATUS_LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite(STATUS_LED, LOW);
  Serial.println("Starting test...");

  for(int row=0; row<=255; row++) 
  {
    Serial.println("Module#1 Testing row: " + String(row));

    writeBits(row,1);
    int numberOfBits = readBits(row,1);
  
    if (numberOfBits != 256) 
    {
      digitalWrite(STATUS_LED, HIGH);
      Serial.println("ERROR: row " + String(row) + " number of bits was: " + String(numberOfBits)) + ", but should be 255.";
      while(1);
    }
  }
  for(int row=0; row<=255; row++) 
  {
    Serial.println("Module#2 Testing row: " + String(row));

    writeBits(row,2);
    int numberOfBits = readBits(row,2);
  
    if (numberOfBits != 256) 
    {
      digitalWrite(STATUS_LED, HIGH);
      Serial.println("ERROR: row " + String(row) + " number of bits was: " + String(numberOfBits)) + ", but should be 255.";
      while(1);
    }    
  }

  Serial.println("Test DONE. All OK!");

  while(1) 
  {
    digitalWrite(STATUS_LED, HIGH);
    delay(500);
    digitalWrite(STATUS_LED, LOW);      
    delay(500);
  }
}

void writeBits(int row, int ras) 
{
  // Pull RAS and CAS HIGH
  if(ras==1)
  {
    digitalWrite(RAS1, HIGH);
  }
  else
  {
    digitalWrite(RAS2, HIGH);  
  }
  digitalWrite(CAS, HIGH);

  // Loop though all the columns
  for (int i=0; i<=255; i++) 
  {
    // Set row address
    digitalWrite(A0, bitRead(row, 0));
    digitalWrite(A1, bitRead(row, 1));
    digitalWrite(A2, bitRead(row, 2));
    digitalWrite(A3, bitRead(row, 3));
    digitalWrite(A4, bitRead(row, 4));
    digitalWrite(A5, bitRead(row, 5));
    digitalWrite(10, bitRead(row, 6));
    digitalWrite(11, bitRead(row, 7));

    // Pull RAS LOW
    if(ras==1)
    {
      digitalWrite(RAS1, LOW);
    }
    else
    {
      digitalWrite(RAS2, LOW);    
    }

    // Pull Write LOW (Enables write)
    digitalWrite(WRITE, LOW);   

    // Set Data in pin to HIGH (write a one)
    digitalWrite(D, HIGH);

    // Set column address
    digitalWrite(A0, bitRead(i, 0));
    digitalWrite(A1, bitRead(i, 1));
    digitalWrite(A2, bitRead(i, 2));
    digitalWrite(A3, bitRead(i, 3));
    digitalWrite(A4, bitRead(i, 4));
    digitalWrite(A5, bitRead(i, 5));
    digitalWrite(10, bitRead(i, 6));
    digitalWrite(11, bitRead(i, 7));

    // Pull CAS LOW
    digitalWrite(CAS, LOW);

    if(ras==1)
    {
      digitalWrite(RAS1, HIGH);
    }
    else
    {
      digitalWrite(RAS2, HIGH); 
    } 
    digitalWrite(CAS, HIGH);
  }
}

int readBits(int row, int ras) 
{

  // Bit counter
  int numberOfBits = 0;

  // Pull RAS, CAS and Write HIGH
  if(ras==1)
  {
    digitalWrite(RAS1, HIGH);
  }
  else
  {
    digitalWrite(RAS2, HIGH); 
  }
    
  digitalWrite(CAS, HIGH);
  digitalWrite(WRITE, HIGH);

  // Loop though all the columns
  for (int i=0; i<=255; i++) 
  {

    // Set row address
    digitalWrite(A0, bitRead(row, 0));
    digitalWrite(A1, bitRead(row, 1));
    digitalWrite(A2, bitRead(row, 2));
    digitalWrite(A3, bitRead(row, 3));
    digitalWrite(A4, bitRead(row, 4));
    digitalWrite(A5, bitRead(row, 5));
    digitalWrite(10, bitRead(row, 6));
    digitalWrite(11, bitRead(row, 7));

    // Pull RAS LOW
    if(ras==1)
    {
      digitalWrite(RAS1, LOW);
    }
    else
    {
      digitalWrite(RAS2, LOW);   
    }

    // Set column address
    digitalWrite(A0, bitRead(i, 0));
    digitalWrite(A1, bitRead(i, 1));
    digitalWrite(A2, bitRead(i, 2));
    digitalWrite(A3, bitRead(i, 3));
    digitalWrite(A4, bitRead(i, 4));
    digitalWrite(A5, bitRead(i, 5));
    digitalWrite(10, bitRead(i, 6));
    digitalWrite(11, bitRead(i, 7));

    // Pull CAS LOW
    digitalWrite(CAS, LOW);

    // Read the stored bit and add to bit counter
    numberOfBits += digitalRead(Q);

    // Pull RAS and CAS HIGH

    if(ras==1)
    {
      digitalWrite(RAS1, HIGH);
    }
    else
    {
      digitalWrite(RAS2, HIGH); 
    } 
    digitalWrite(CAS, HIGH);
  }

  return numberOfBits;
}
