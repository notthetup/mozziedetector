#include <Wire.h>

#define CPU_FREQ 16000000L
#define TWI_FREQ 400000L

#define ADDR_ADC121             0x55

#define V_REF 3.00

#define REG_ADDR_RESULT         0x00
#define REG_ADDR_ALERT          0x01
#define REG_ADDR_CONFIG         0x02
#define REG_ADDR_LIMITL         0x03
#define REG_ADDR_LIMITH         0x04
#define REG_ADDR_HYST           0x05
#define REG_ADDR_CONVL          0x06
#define REG_ADDR_CONVH          0x07

#define LOG_SIZE 250

unsigned int getData;
unsigned long times[LOG_SIZE];
int index = 0;
int avail = 0;

bool writeAddrInit = false;

float analogVal=0;         // convert
void init_adc()
{
  Serial.println("Initing ADC");
  Wire.beginTransmission(ADDR_ADC121);        // transmit to device
  Wire.write(REG_ADDR_CONFIG);                // Configuration Register
  Wire.write(0x20);                           // Set Automatic Conversion Mode '001' - 27ksps
  Wire.endTransmission();
}

void read_adc()     //unsigned int *data
{  
    if (writeAddrInit == false){
      Wire.beginTransmission(ADDR_ADC121);        // transmit to device
      Wire.write(REG_ADDR_RESULT);                // get reuslt
      Wire.endTransmission();
      writeAddrInit = true;
    }
   
    Wire.requestFrom(ADDR_ADC121, 2, false);           // request 2byte from device
    getData = (Wire.read()&0x0f)<<8;
    getData |= Wire.read();
    times[index] = micros();
    index++;
    if (index >= LOG_SIZE){
      index = 0;
    }
    //Serial.print("getData:");
    //Serial.println(getData);
}
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Begin");
  //TWBR = ((CPU_FREQ / TWI_FREQ) - 16) / 2;
  Serial.println("Init");
  init_adc();
}

void loop()
{  
  read_adc();//adcRead);
   //delay(50);
   if (index == 0){
     for (int i = 0; i < LOG_SIZE; i++){
       Serial.println(times[i]);   
     }
   }
}
