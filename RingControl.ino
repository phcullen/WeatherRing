// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

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

//enable ring
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

//Define Global Variables
int i;//counter
long previousMillis = 0;//initiation of coumter for weather data pull
long interval = 300000; //Interval for pulling weather data milliseconds
int currentTemp = NULL; //Current temprature F
int windSpeed = NULL;//Winf speed MPH
String weatherState = "";
char forcast='er';
int wSpeed=60;//windspeed led delay milliseconds


void setup() {
    //Testing Conditions
    forcast='r';//sunny,clear, rainy, etc.
    currentTemp=100;//temprature F
    wSpeed=200;//windspeed delay
    
  strip.begin();
  strip.show();
  strip.setBrightness(35);
  
  // Subscribe to the integration response event
  Particle.subscribe(System.deviceID() + "_test1", WuHandler, MY_DEVICES);
}

void loop() {
   unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis > interval){
    // Get some data
    String data = String(10);
    // Trigger the integration
    Particle.publish("test1", data);
    previousMillis = currentMillis;
  }
  
  
  
  
  
  
  if(weatherState=="Clear"||weatherState=="Partly Cloudy"||weatherState=="Scattered Clouds"){
    forcast='s';//Sunny
  }
  else if(weatherState=="Light Drizzle"||weatherState=="Drizzle"||weatherState=="Heavy Drizzle"||weatherState=="Light Rain"||weatherState=="Rain"||weatherState=="Heavy Rain"){
    forcast='r';//Rainy
  }
  else if(weatherState=="Light Snow"||weatherState=="Snow"||weatherState=="Heavy Snow"||weatherState=="Light Freezing Rain"||weatherState=="Freezing Rain"||weatherState=="Hevy Freezing Rain"){
    forcast='w';//Snowy
  }
  else if(weatherState=="Mostly Cloudy"||weatherState=="Overcast"||weatherState=="Cloudy"||weatherState=="Fog"){
    forcast='c';//Cloudy
  }
  else{
    forcast='e';//Error
  }
    
    
    
     
    if(currentTemp<55){//temprature less than 55F
        switch(forcast){
            case 's': //Sunny
                cycle(YELLOW,BLUE,wSpeed);
                break;
            case 'r'://Rainy
                cycle(CYAN,BLUE,wSpeed);
                break;
            case 'w'://snowing
                cycle(WHITE,BLUE,wSpeed);
                break;
            case 'c'://cloudy
                cycle(BLACK,BLUE,wSpeed);
                break;
            case 'e'://cloudy
                cycle(RED,PURPLE,wSpeed);
                break;
        }
    }
    else if(currentTemp>77){//temprature greater than 77F
        switch(forcast){
            case 's': //Sunny
                cycle(YELLOW,PEACH,wSpeed);
                break;
            case 'r'://Rainy
                cycle(CYAN,PEACH,wSpeed);
                break;
            case 'w'://snowing
                cycle(WHITE,PEACH,wSpeed);
                break;
            case 'c'://Cloudy
                cycle(BLACK,PEACH,wSpeed);
                break;
            case 'e'://error
                cycle(RED,PURPLE,wSpeed);
                break;
        }
    }
    else{//temprature between 55F and 77F
        switch(forcast){
            case 's': //Sunny
                cycle(YELLOW,GREEN,wSpeed);
                break;
            case 'r'://Rainy
                cycle(CYAN,GREEN,wSpeed);
                break;
            case 'w'://snowing
                cycle(WHITE,GREEN,wSpeed);
                break;
            case 'c'://Cloudy
                cycle(BLACK,GREEN,wSpeed);
                break;
            case 'e'://cloudy
                cycle(RED,PURPLE,wSpeed);
                break;
        }
    }
}
//light animation function
void cycle(int R1, int G1, int B1, int R2, int G2, int B2,int wSpeed){
    for(i=0; i < PIXEL_COUNT; i++) {
        strip.setPixelColor(i, R1, G1, B1);
        strip.show();
        delay(wSpeed);
        strip.setPixelColor(i, R2, G2, B2);
        strip.show();
        
    }
}    
//retreving data from WUnderground
void WuHandler(const char *event, const char *data) {
  // Handle the integration response

    String str = String(data);
    char strBuffer[64] = "";
    str.toCharArray(strBuffer, 64);
    
    weatherState = strtok(strBuffer, "\"~");
    //float currentTemp = atof(strtok(NULL, "~"));
    currentTemp = atoi(strtok(NULL, "~"));
    //String humidity = strtok(NULL, "~");
    //String windDir = strtok(NULL, "~");
    windSpeed = atoi(strtok(NULL, "~")); //convert from string float to integer to eliminate fractional wind speed
    //String dewPoint = strtok(NULL, "~");
    
    
}


//Possible Conditions from https://www.wunderground.com/weather/api/d/docs?d=resources/phrase-glossary&_ga=2.154336062.747331980.1509202850-1348394414.1509056105&MR=1
/*
[Light/Heavy] Drizzle
[Light/Heavy] Rain
[Light/Heavy] Snow
[Light/Heavy] Snow Grains
[Light/Heavy] Ice Crystals
[Light/Heavy] Ice Pellets
[Light/Heavy] Hail
[Light/Heavy] Mist
[Light/Heavy] Fog
[Light/Heavy] Fog Patches
[Light/Heavy] Smoke
[Light/Heavy] Volcanic Ash
[Light/Heavy] Widespread Dust
[Light/Heavy] Sand
[Light/Heavy] Haze
[Light/Heavy] Spray
[Light/Heavy] Dust Whirls
[Light/Heavy] Sandstorm
[Light/Heavy] Low Drifting Snow
[Light/Heavy] Low Drifting Widespread Dust
[Light/Heavy] Low Drifting Sand
[Light/Heavy] Blowing Snow
[Light/Heavy] Blowing Widespread Dust
[Light/Heavy] Blowing Sand
[Light/Heavy] Rain Mist
[Light/Heavy] Rain Showers
[Light/Heavy] Snow Showers
[Light/Heavy] Snow Blowing Snow Mist
[Light/Heavy] Ice Pellet Showers
[Light/Heavy] Hail Showers
[Light/Heavy] Small Hail Showers
[Light/Heavy] Thunderstorm
[Light/Heavy] Thunderstorms and Rain
[Light/Heavy] Thunderstorms and Snow
[Light/Heavy] Thunderstorms and Ice Pellets
[Light/Heavy] Thunderstorms with Hail
[Light/Heavy] Thunderstorms with Small Hail
[Light/Heavy] Freezing Drizzle
[Light/Heavy] Freezing Rain
[Light/Heavy] Freezing Fog
Patches of Fog
Shallow Fog
Partial Fog
Overcast
Clear
Partly Cloudy
Mostly Cloudy
Scattered Clouds
Small Hail
Squalls
Funnel Cloud
Unknown Precipitation
Unknown
*/