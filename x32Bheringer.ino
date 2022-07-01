
//data used later
//I was lazy so I put it all in one file

byte input1[] = {
  0xB1, 0x50, 0x00,
  0xB1, 0x00, 0x7f,
  0xB1, 0x01, 0x7f,
  0xB1, 0x02, 0x7f,
  0xB1, 0x03, 0x7f,
  0xB1, 0x04, 0x7f,
  0xB1, 0x05, 0x7f,
  0xB1, 0x06, 0x7f,
  0xB1, 0x07, 0x7f
};
byte input2[] = {
  0xB1, 0x50, 0x7F,
  0xB1, 0x00, 0x00,
  0xB1, 0x01, 0x00,
  0xB1, 0x02, 0x00,
  0xB1, 0x03, 0x00,
  0xB1, 0x04, 0x00,
  0xB1, 0x05, 0x00,
  0xB1, 0x06, 0x00,
  0xB1, 0x07, 0x00
};

byte input3[] = {
  0xB1, 0x51, 0x00,
  0xB1, 0x28, 0x7F,
  0xB1, 0x29, 0x7F,
  0xB1, 0x2A, 0x7F,
  0xB1, 0x2B, 0x7F,
  0xB1, 0x2C, 0x7F,
  0xB1, 0x2D, 0x7F,
  0xB1, 0x2E, 0x7F,
  0xB1, 0x2F, 0x7F
};
byte input4[] =
{ 0xB1, 0x51, 0x7F,
  0xB1, 0x28, 0x00,
  0xB1, 0x29, 0x00,
  0xB1, 0x2A, 0x00,
  0xB1, 0x2B, 0x00,
  0xB1, 0x2C, 0x00,
  0xB1, 0x2D, 0x00,
  0xB1, 0x2E, 0x00,
  0xB1, 0x2F, 0x00
};

byte input5[] =
{ 0xB1, 0x52, 0x00,
  0xB1, 0x20, 0x7F,
  0xB1, 0x21, 0x7F
};
byte input6[] =
{ 0xB1, 0x52, 0x7F,
  0xB1, 0x20, 0x00,
  0xB1, 0x21, 0x00
};

byte input7[] =
{ 0xB1, 0x53, 0x00,
  0xB1, 0x10, 0x7F
};

byte input8[] =
{ 0xB1, 0x53, 0x7F,
  0xB1, 0x10, 0x00
};


bool Dstate[6] = {0, 0, 0, 0, 0, 0};//all the buttons states
int Astate[4] = {0, 0, 0, 0};//all the potentiometers states
int a0 = 0;
bool clicking[6] = {false, false, false, false, false, false};

int microsa = 0;
int microsd[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(31250);//31250 for midi
  for (int i = 2; i < 8; i++)
  {
    pinMode(i, INPUT);
  }

  for (int i = 14; i < 17; i++)
  {
    pinMode(i, INPUT);
  }
}

void loop() {



  ////////////// read buttons /////////////////////
  for (int i = 2; i < 6; i++)
  {
    int  curr_micros = micros();
    if (curr_micros - microsd[i - 2] > 20000)
    {
      int input = digitalRead(i);
      if (input)
      {
        if (!clicking[i - 2])
        {
          clicking[i - 2] = true;

          if (Dstate[i - 2] == false)
            Dstate[i - 2] = true;
          else Dstate[i - 2] = false;
          handleD(i, Dstate[i - 2]);
        }
      }
      else if (clicking[i - 2])
      {
        microsd[i - 2] = micros();
        clicking[i - 2] = false;
      }
    }
  }
  
  /////////// read potentiometers /////////////////

  int newm = micros();

  if ((newm - microsa) > 32000) //sampling frequency
  { microsa = newm;

    for (int i = 14; i < 16; i++) //18
    {
      int input = analogRead(i);

      if (abs(Astate[i - 14] - input) > 6)
      {
        Astate[i - 14] = input;
        if (abs(1023 - Astate[i - 14]) < 12)
        {
          Astate[i - 14] = 1023;
        }
        if (abs(Astate[i - 14]) < 12)
        {
          Astate[i - 14] = 0;
        }
        handleA(i, Astate[i - 14]);
      }
    }


  }

}//end of loop



void handleA(int pin, int input)
{

  int result = (input / 8);

  switch (pin) {
    case 14:


      Serial.write(0xB0);
      Serial.write(0x20);
      Serial.write(result);
      Serial.write(0xB0 );
      Serial.write(0x21 );
      Serial.write(result);

      //Serial.println(result);

      break;
    case 15:

      Serial.write(0xB0);
      Serial.write(0x46);
      Serial.write(result);

//Serial.println(result);
      break;
  }
}





void handleD(int pin, bool state)
{
  switch (pin) {
    case 2:
      if (state)
      {
        Serial.write(input1, 27);
      }
      else
      { Serial.write(input2, 27);
      }
      break;

    case 3:
      if (state)
      {
        Serial.write(input3, 27);
      }
      else
      { Serial.write(input4, 27);
      }
      break;
    case 4:
      if (state)
      {
        Serial.write(input5, 9);
      }
      else {
        Serial.write( input6, 9);
      }
      break;
    case 5:
      if (state)
      {
        Serial.write(input7, 6);
      }
      else
      {
        Serial.write(input8, 6);
      }
      break;
  }
}
