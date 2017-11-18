
SYSTEM_THREAD(ENABLED);//program doesnt lag for loss internet

// Add libraries
#include <neopixel.h>
#include <ThingSpeak.h>
#include <tuple>//functions with multiple outputs

// Set pixel COUNT, PIN and TYPE
#define PIXEL_COUNT 24
#define PIXEL_PIN D6
#define PIXEL_TYPE WS2812



//enable ring
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


///////establish connection with ThingSpeak////////////////////////////////////////////////////////
TCPClient client;
unsigned int myChannelNumber = xxxxxx; //ChannelID
const char * myWriteAPIKey = "Your ThingSpeak api key"; // APIKey




//time intervals for delayed functions//////////////////////////////////////////////////////////////////////////////
long intervalWUPull = 60000; //Interval for pulling weather data from WU milliseconds
long intervalDataPost = 60000; //Interval for pulling weather data from WU milliseconds

//initilize functions
//void WUretreve();
//void WuHandler(const char *event, const char *data);
//uint32_t GetCondition(String current);
//uint32_t GetTemprature(int temp);
//void Cycle(uint32_t conditionTemp, uint32_t colorTemp,int wSpeed);
//int WindSpeed(int windSpeed);
//void PostData(int temp,int wSpeed);


////////////Color definitions///////////////////////////////////////////////////////////////////////////////////////
uint32_t PURPLE=strip.Color(180,3,180);
uint32_t PEACH=strip.Color(200,50,5);
uint32_t CYAN=strip.Color(10,150,70);
uint32_t BLUE=strip.Color(5,5,190);
uint32_t WHITE=strip.Color(150,150,150);
uint32_t GREEN=strip.Color(10,180,10);
uint32_t YELLOW=strip.Color(255,255,0);
uint32_t BLACK=strip.Color(0,0,0);
uint32_t RED=strip.Color(255,0,0);

//Define Global Variables/////////////////////////////////////////////////////////////////////////////////////////
int currentTemp = NULL; //Current temprature F
int windSpeed = NULL;//Winf speed MPH
String humidity ="";
String weatherState = "";

int TTTTT=99;
int Humidity=NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
    
    //initiate LED control
    strip.begin();
    strip.show();
    strip.setBrightness(100);//set brightness limit of LEDs
  
    // Subscribe to the integration response event
    Particle.subscribe(System.deviceID() + "_test1", WuHandler, MY_DEVICES);
    
    Particle.subscribe("Phcullen_DHT_Humidity_Post", HHandler);
    Particle.subscribe("Phcullen_DHT_Temprature_Post", THandler);
    //start ThinkSpeak client
    ThingSpeak.begin(client);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
   
   
    WUretreve();//retreve WU data
    PostData( ,windSpeed);//Post data to ThingSpeak
    Cycle(GetCondition(weatherState),GetTemprature(TTTTT),WindSpeed(windSpeed));//run animation
    //Particle.publish("Test_Temprature_Postx",String(TTTTT));
    //Particle.publish("Test_Temprature_Postxx",String(Humidity));
    
   
}


//call to wunderground and initiate WuHandler//////////////////////////////////////////////////////////////
void WUretreve(){
    unsigned long currentMillis = millis();
    static long previousMillisWU = 0;//initiation of coumter for weather data pull
    
    if(currentMillis - previousMillisWU > intervalWUPull){
    // Get some data
    String data = String(10);
    // Trigger the integration
    Particle.publish("test1", data);
    previousMillisWU = currentMillis;
  }
}  


//retreve and parse data from WUnderground///////////////////////////////////////////////////////////////////////////
void WuHandler(const char *event, const char *data) {
  // Handle the integration response

    String str = String(data);
    char strBuffer[64] = "";
    str.toCharArray(strBuffer, 64);
    
    weatherState = strtok(strBuffer, "\"~");
    //float currentTemp = atof(strtok(NULL, "~"));
    currentTemp = atoi(strtok(NULL, "~"));//retreve temp F
    humidity = strtok(NULL, "~");
    String windDir = strtok(NULL, "~");
    windSpeed = atoi(strtok(NULL, "~")); //convert from string float to integer to eliminate fractional wind speed MPH
    String dewPoint = strtok(NULL, "~");
    
    
}

void THandler(const char *event, const char *data){
    TTTTT = atoi(data);
    //Humidity = atoi(strtok (NULL, "~"));
    //Particle.publish("Test_Temprature_Post",data);
    
}

void HHandler(const char *event, const char *data){
    Humidity = atoi(String(data));
    //Particle.publish("Test_Humidity_Post",data);
  
}

//recieve current condition from WU and return a color for tracking LED////////////////////////////////////////////////////
uint32_t GetCondition(String current){
    
    const int x=6;
    const int y=50;
    bool done=false;
        
        
    String conditions[x][y]={
        {"RAIN","Light Drizzle","Drizzle","Heavy Drizzle","Light Rain","Rain","Heavy Rain","Light Mist","Mist","Heavy Mist","Light Rain Showers","Rain Showers","Heavy Rain Showers","Light Thunderstorm","Thunderstorm","Heavy Thunderstorm","Light Thunderstorms and Rain","Thunderstorms and Rain","Heavy Thunderstorms and Rain","Light Rain Mist","Rain Mist","Heavy Rain Mist","Light Freezing Drizzle","Freezing Drizzle","Heavy Freezing Drizzle","Light Freezing Rain","Freezing Rain","Heavy Freezing Rain","Squalls","Light Spray","Spray","Heavy Spray"},
        {"CLEAR","Clear","Partly Cloudy","Scattered Clouds","Shallow Fog","Patches of Fog"},
        {"SNOW","Light Snow","Snow","Heavy Snow","Light Snow Grains","Snow Grains","Heavy Snow Grains","Light Ice Crystals","Ice Crystals","Heavy Ice Crystals","Light Ice Pellets","Ice Pellets","Heavy Ice Pellets","Light Hail","Hail","Heavy Hail","Light Low Drifting Snow","Low Drifting Snow","Heavy Low Drifting Snow","Light Blowing Snow","Blowing Snow","Heavy Blowing Snow","Light Snow Showers","Snow Showers","Heavy Snow Showers","Light Snow Blowing Snow Mist","Snow Blowing Snow Mist","Heavy Snow Blowing Snow Mist","Light Ice Pellet Showers","Ice Pellet Showers","Heavy Ice Pellet Showers","Light Hail Showers","Hail Showers","Heavy Hail Showers","Light Small Hail Showers","Small Hail Showers","Heavy Small Hail Showers","Light Thunderstorms and Snow","Thunderstorms and Snow","Heavy Thunderstorms and Snow","Light Thunderstorms and Ice Pellets","Thunderstorms and Ice Pellets","Heavy Thunderstorms and Ice Pellets","Light Thunderstorms with Hail","Thunderstorms with Hail","Heavy Thunderstorms with Hail","Light Thunderstorms with Small Hail","Thunderstorms with Small Hail","Heavy Thunderstorms with Small Hail","Small Hail"},
        {"CLOUDY","Light Fog","Fog","Heavy Fog","Light Fog Patches","Fog Patches","Heavy Fog Patches","Light Freezing Fog","Freezing Fog","Heavy Freezing Fog","Partial Fog","Overcast","Mostly Cloudy"},
        {"BAD","Light Smoke","Smoke","Heavy Smoke","Funnel Cloud","Light Volcanic Ash","Volcanic Ash","Heavy Volcanic Ash","Light Widespread Dust","Widespread Dust","Heavy Widespread Dust","Light Sand","Sand","Heavy Sand","Light Dust Whirls","Dust Whirls","Heavy Dust Whirls"},
        {"??","Unknown","Unknown Precipitation"}
    };


    for(int i=0;i<x&& !done;i++){
        for(int j=0;j<y&&conditions[i][j]!="";j++){
            
            
            if(current==conditions[i][j]){              
                done=true;
                if(conditions[i][0]=="RAIN"){
                    return CYAN;
                    
                }else if(conditions[i][0]=="CLEAR"){
                    return YELLOW;
                    
                }else if(conditions[i][0]=="SNOW"){
                    return WHITE;
                    
                }else if(conditions[i][0]=="CLOUDY"){
                    return BLACK;
                    
                }else if(conditions[i][0]=="BAD"){
                    return PEACH;
                    
                }else if(conditions[i][0]=="??"){
                    return PEACH;
                    
                }else{
                    return RED;
                }
                
            }

                
        }
    }
}

//recieve current temprature from WU and return base LED color//////////////////////////////////////////////////////////////////
uint32_t GetTemprature(int temp){
    
    //aray of colorsr from blur to green to red for temprature display
    uint32_t spectrum[50]={strip.Color(0,3,229),strip.Color(0,22,228),strip.Color(0,40,227),strip.Color(0,59,226),strip.Color(0,77,225),strip.Color(0,95,225),strip.Color(0,113,224),strip.Color(0,131,223),strip.Color(0,148,222),strip.Color(0,166,222),strip.Color(0,183,221),strip.Color(0,200,220),strip.Color(0,218,219),strip.Color(0,218,202),strip.Color(0,218,184),strip.Color(0,217,165),strip.Color(0,216,147),strip.Color(0,215,129),strip.Color(0,215,111),strip.Color(0,214,93),strip.Color(0,213,76),strip.Color(0,212,58),strip.Color(0,211,41),strip.Color(0,211,23),strip.Color(0,210,6),strip.Color(10,209,0),strip.Color(27,208,0),strip.Color(44,208,0),strip.Color(60,207,0),strip.Color(77,205,0),strip.Color(93,205,0),strip.Color(110,204,0),strip.Color(126,204,0),strip.Color(142,203,0),strip.Color(158,202,0),strip.Color(174,201,0),strip.Color(189,201,0),strip.Color(200,195,0),strip.Color(199,178,0),strip.Color(198,161,0),strip.Color(197,144,0),strip.Color(197,128,0),strip.Color(196,111,0),strip.Color(195,95,0),strip.Color(194,78,0),strip.Color(194,63,0),strip.Color(193,47,0),strip.Color(192,31,0),strip.Color(191,15,0),strip.Color(191,0,0)};
    temp=temp/2;
    
    for(int i=0;i<50;i++){
        if (temp==i){
            
            return spectrum[i];
        }else if(temp>50) {
            return PURPLE;
        }
    }
}

//light animation function/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cycle(uint32_t conditionTemp, uint32_t colorTemp,int wSpeed){
    for(int i=0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, conditionTemp);
        strip.show();
        delay(wSpeed);
        strip.setPixelColor(i, colorTemp);
        strip.show();
        
    }
}  

//Wind speed translator function/////////////////////////////////////////////////////////////////////////////////////////////////////
int WindSpeed(int windSpeed){
    int wSpeed=0;//windspeed led delay milliseconds
    
    if(windSpeed<1){
        wSpeed=400;
    }
    else if(windSpeed<5){
        wSpeed=380;
    }
    else if(windSpeed<8){
        wSpeed=350;
    }
    else if(windSpeed<12){
        wSpeed=320;
    }
    else if(windSpeed<19){
        wSpeed=290;
    }
    else if(windSpeed<25){
        wSpeed=260;
    }
    else if(windSpeed<32){
        wSpeed=230;
    }
    else if(windSpeed<39){
        wSpeed=200;
    }
    else if(windSpeed<47){
        wSpeed=170;
    }
    else if(windSpeed<55){
        wSpeed=140;
    }
    else if(windSpeed<64){
        wSpeed=110;
    }
    else if(windSpeed<74){
        wSpeed=80;
    }
    else{
        wSpeed=50;
    }
    
return wSpeed;}

////Posts data to ThingSpeak/////////////////////////////////////////////////////////////
void PostData(int temp,int wSpeed){
    unsigned long currentMillis = millis();
    static long previousMillisWU = 0;//initiation of coumter for data post interval 
    if(currentMillis - previousMillisWU > intervalDataPost){
        //Particle.publish("tempraturePost", String(temp), PRIVATE);
        ThingSpeak.setField(1,temp);
        ThingSpeak.setField(2,wSpeed);
        ThingSpeak.setField(3,TTTTT);
        //ThingSpeak.setField(7,Humidity);
        //ThingSpeak.setField(3,humidity);
        ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        previousMillisWU = currentMillis;
    }
}
