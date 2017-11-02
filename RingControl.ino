// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>
#include <tuple>//functions with multiple outputs

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_COUNT 24
#define PIXEL_PIN D6
#define PIXEL_TYPE WS2812

//Color definitions
#define PEACH 200,50,5
#define CYAN 10,150,70
#define PURPLE 180,3,180
#define BLUE 5,5,190
#define WHITE 150,150,150
#define GREEN 10,180,10
#define YELLOW 255,255,0
#define BLACK 0,0,0
#define RED 255,0,0

//Color sets
#define TYPE R1,G1,B1
//#define TEMP R2,G2,B2

//enable ring
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

//Define Global Variables
int currentTemp = NULL; //Current temprature F
int windSpeed = NULL;//Winf speed MPH
String weatherState = "";
//char forcast="";

long previousMillisWU = 0;//initiation of coumter for weather data pull
long interval = 60000; //Interval for pulling weather data from WU milliseconds

//initilize functions
std::tuple<int, int,int > GetCondition(String current);
//std::tuple<int, int,int > GetTemprature(int temp);


///////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
    //Testing Conditions
        //forcast='r';//sunny,clear, rainy, etc.
        //currentTemp=100;//temprature F
        //wSpeed=200;//windspeed delay
    
  strip.begin();
  strip.show();
  strip.setBrightness(100);
  
  // Subscribe to the integration response event
  Particle.subscribe(System.deviceID() + "_test1", WuHandler, MY_DEVICES);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
   
   int TYPE,TEMP,wSpeed;
  
    WUretreve();//retreve WU data
    
   // WindSpeed(windSpeed);//convert wind speed to delay interverl
    std::tie(TYPE)=GetCondition(weatherState);  //retreve condition color
    //std::tie(TEMP)=GetTemprature(currentTemp); //retreve temprature color
    //uint32_t Temp=GetTemprature(currentTemp);
    //wSpeed=400;
    Cycle(TYPE,GetTemprature(currentTemp),WindSpeed(windSpeed));//run animation
    
    

   
}


//call to wunderground and initiate WuHandler//////////////////////////////////////////////////////////////
void WUretreve(){
    unsigned long currentMillis = millis();
    
    if(currentMillis - previousMillisWU > interval){
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
    //String humidity = strtok(NULL, "~");
    //String windDir = strtok(NULL, "~");
    windSpeed = atoi(strtok(NULL, "~")); //convert from string float to integer to eliminate fractional wind speed MPH
    //String dewPoint = strtok(NULL, "~");
    
    
}

//recieve current condition from WU and return a color for tracking LED////////////////////////////////////////////////////
std::tuple<int, int,int > GetCondition(String current){
    
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
                    return std::make_tuple(CYAN);
                    
                }else if(conditions[i][0]=="CLEAR"){
                    return std::make_tuple(YELLOW);
                    
                }else if(conditions[i][0]=="SNOW"){
                    return std::make_tuple(WHITE);
                    
                }else if(conditions[i][0]=="CLOUDY"){
                    return std::make_tuple(BLACK);
                    
                }else if(conditions[i][0]=="BAD"){
                    return std::make_tuple(PEACH);
                    
                }else if(conditions[i][0]=="??"){
                    return std::make_tuple(PEACH);
                    
                }else{
                    return std::make_tuple(RED);
                }
                
            }

                
        }
    }
}

//recieve current temprature from WU and return base LED color//////////////////////////////////////////////////////////////////
uint32_t GetTemprature(int temp){
    
    uint32_t spectrum[50]={strip.Color(0,3,229),strip.Color(0,22,228),strip.Color(0,40,227),strip.Color(0,59,226),strip.Color(0,77,225),strip.Color(0,95,225),strip.Color(0,113,224),strip.Color(0,131,223),strip.Color(0,148,222),strip.Color(0,166,222),strip.Color(0,183,221),strip.Color(0,200,220),strip.Color(0,218,219),strip.Color(0,218,202),strip.Color(0,218,184),strip.Color(0,217,165),strip.Color(0,216,147),strip.Color(0,215,129),strip.Color(0,215,111),strip.Color(0,214,93),strip.Color(0,213,76),strip.Color(0,212,58),strip.Color(0,211,41),strip.Color(0,211,23),strip.Color(0,210,6),strip.Color(10,209,0),strip.Color(27,208,0),strip.Color(44,208,0),strip.Color(60,207,0),strip.Color(77,205,0),strip.Color(93,205,0),strip.Color(110,204,0),strip.Color(126,204,0),strip.Color(142,203,0),strip.Color(158,202,0),strip.Color(174,201,0),strip.Color(189,201,0),strip.Color(200,195,0),strip.Color(199,178,0),strip.Color(198,161,0),strip.Color(197,144,0),strip.Color(197,128,0),strip.Color(196,111,0),strip.Color(195,95,0),strip.Color(194,78,0),strip.Color(194,63,0),strip.Color(193,47,0),strip.Color(192,31,0),strip.Color(191,15,0),strip.Color(191,0,0)};
    temp=temp/2;
    
    for(int i=0;i<50;i++){
        if (temp==i){
            
            return spectrum[i];
        }
    }
    
    /*
    if(currentTemp<55){//temprature less than 55F
        return std::make_tuple(BLUE);
    }
    else if(currentTemp>77){//temprature greater than 77F
        return std::make_tuple(PEACH);
    }
    else{//temprature between 55F and 77F
        return std::make_tuple(GREEN);
    }
    
    
    
    end goal put 100 colors in a gradiant and loop through until temprature == i and display color */
}

//light animation function/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cycle(int R1, int G1, int B1, uint32_t colorTemp,int wSpeed){
    for(int i=0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, R1, G1, B1);
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
    if(windSpeed<5){
        wSpeed=380;
    }
    if(windSpeed<8){
        wSpeed=350;
    }
    if(windSpeed<12){
        wSpeed=320;
    }
    if(windSpeed<19){
        wSpeed=290;
    }
    if(windSpeed<25){
        wSpeed=260;
    }
    if(windSpeed<32){
        wSpeed=230;
    }
    if(windSpeed<39){
        wSpeed=200;
    }
    if(windSpeed<47){
        wSpeed=170;
    }
    if(windSpeed<55){
        wSpeed=140;
    }
    if(windSpeed<64){
        wSpeed=110;
    }
    if(windSpeed<74){
        wSpeed=80;
    }
    else{
        wSpeed=50;
    }
    
return wSpeed;}