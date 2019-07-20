/*
   ============================
   Maker_LED_Matrix_example.ino
   ============================
   
   This is example code on how to use Maker LED Matrix Display library on an e-radionica Maker LED Matrix Display Board with ESP8266
   and two or four IL31LF3731 drivers.
  
   It shows how to print text/messages to the display, write a picture, scroll it, pause it and remove it.
  
   It also shows how to use functions from Adafruit GFX to draw lines, circles, rects, bitmaps, text and scroll all that.
*/


#include "Maker_LED_Matrix.h"       //First, we have to include this library to our sketch!

/*
   Next we make an object to a class. We call this object 'matrix', but you can name it whatever you want. But remember to use 
   that name instead of 'matrix' in the rest of the code!
   
   Only ONE of the 'Maker_LED_Matrix matrix()' lines below can be specified. If you uncomment one by removing the leading '//'
   then be sure to comment the other one by adding the leading '//'.
   
   Use MAKER_LED_MATRIX_2 if you're using Maker LED Matrix 2 with resolution of 32x9.
   Use MAKER_LED_MATRIX_4 if you're using Maker LED Matrix 4 with resolution of 64x9.
*/

//Maker_LED_Matrix matrix(MAKER_LED_MATRIX_2);
Maker_LED_Matrix matrix(MAKER_LED_MATRIX_4);

/*
   Let's make a bitmap picture of a sine wave. To convert your bitmap image to bytes, follow this tutorial:
   https://learn.adafruit.com/steampunk-cameo-necklace/bitmaps-and-code
*/
static const uint8_t sine[] PROGMEM = {
  0x03, 0xc0, 0x00, 0x00,
  0x0c, 0x30, 0x00, 0x00,
  0x30, 0x0c, 0x00, 0x00,
  0x40, 0x02, 0x00, 0x00,
  0x80, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xc0, 0x03,
  0x00, 0x00, 0x20, 0x0c,
  0x00, 0x00, 0x1c, 0x70,
  0x00, 0x00, 0x03, 0x80,
};


/*
   And now, let's make an 8 bit grayscale bitmap! To convert your picture to an array of bytes like the flame[] one below,
   follow these instructions!
  
    1. Download LCD Image Converter: http://sourceforge.net/projects/lcd-image-converter/files/
    2. Open program->New Image->Type the image name to be used as the name of the data array (e.g., flame)->click OK
    3. Go to Image->Import...
    4. Select the image file that you want to convert. Note that the image should not be bigger than the display,
       either 32x9 pixels or 64x9 pixels. If it is bigger, use Microsoft Paint to resize it.
    5. Go to Options->Conversion...
    6. At the very top left, make sure Preset = Grayscale 8
    7. On the Prepare tab, make sure
       - Type = Grayscale
       - Inverse = CHECKED
       - Main Scan Direction = Top to Bottom
       - Line Scan Direction = Forward
    8. On the Image tab, make sure
       - Split to rows = CHECKED
       - Block size = 8 bit
       - Byte order = Little-Endian
    9. By clicking Show Preview, you should see the image as well as the data array. 
       The colors on the image should be inverted.
       Click OK to close the preview.
   10. Click OK and you'll be asked, "Save changes?", click YES. This saves a temporary copy in memory only.
   11. Go to File->Convert... and save your data array to a file.
   12. Open it in Notepad, select everything from 'static' to the end, and copy and paste it over the 'flame' data array below.
   13. Be sure to insert the word PROGMEM before the equal sign.
       (See https://www.arduino.cc/reference/en/language/variables/utilities/progmem/ for more info.)
   14. If your array is not called 'flame' be sure to change it in the rest of the code!
*/
static const uint8_t flame[288] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x69, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x93, 0x93, 0x93, 0x00, 0x00, 0x00, 0x93, 0x69, 0x69, 0x93, 0x93, 0x93, 0x00, 0x93, 0x93, 0x69, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x93, 0x93, 0x93, 0x93, 0x69, 0x93, 0x93, 0x93, 0x93, 0x69, 0x69, 0x69, 0x69, 0x93, 0x93, 0x93, 0x69, 0x69, 0x93, 0x93, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x93, 0x93, 0x93, 0x93, 0x69, 0x69, 0x3f, 0x69, 0x69, 0x69, 0x69, 0x3f, 0x3f, 0x69, 0x69, 0x69, 0x69, 0x3f, 0x69, 0x69, 0x93, 0x93, 0x93, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x93, 0x93, 0x69, 0x69, 0x3f, 0x3f, 0x0f, 0x0f, 0x3f, 0x3f, 0x3f, 0x0f, 0x3f, 0x3f, 0x3f, 0x0f, 0x3f, 0x3f, 0x69, 0x69, 0x69, 0x93, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x93, 0x93, 0x69, 0x69, 0x3f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x3f, 0x69, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void setup() {
  Serial.begin(9600);        //Fire up Serial monitor at 9600 baud.
   
  matrix.begin(&matrix);     //Init our library. For the argument we are sending the address of our library object.
                             //It's necessary to do this, otherwise, we are unable to refresh the LED Matrix.

  matrix.brightness(25, 1);  //Setup font (foreground) brightness at 25 of 255.
                             //Setup background brightness at 1 of 255 (this turns on the backlight effect, put zero 
                             //to disable it).

  /*
     Finally, let's write a text message to the display. Let's set a 75 ms delay between steps, make two pixel steps, and 
     only one repeat of the message. If you want to repeat this message all the time, set the repeat value to -1 instead of 1.
     And if you want it to be static (does not scroll) set the repeat value to zero.
  */
  matrix.message("Hello! :) This is Maker LED Matrix Display made by e-radionica.com", 75, 2, 1);
 
  while(matrix.repeatCount() != 1) {  //Wait until the number of repeats is exactly one, which means we've reached
                                      //the end of the message.
    delay(100);
  }
  
  matrix.picture(sine, 32, 9, 100, 1, 2); //Message is boring, let's write a scrolling picture to display with dimensions
                                          //of 32x9 pixels, 100 ms pause between steps, one pixel step and only two repeats! :D
  delay(4000);
  matrix.stopScroll();   //Want to pause the scrolling? No problem! Just call this function.
  delay(2000);
  matrix.resumeScroll(); //After a little break, let's continue with scrolling.
  delay(4000);
  matrix.picture(sine, 32, 9, 100, 1, 0); //And if you want it to be a static image, without scrolling, just set repeats to zero, easy.
  delay(2000);

  /*
     Now, this is important! You can have a background color (aka "backlight") but, the background color of the image would 
     be black, so it's better to turn off the background color.
  */
  matrix.brightness(64, 0); //Font brightness also affects brightness of image.
  
  matrix.picture8Bit(flame, 32, 9, 75, 1, -1);  //Write your 8 bit grayscale image to the display.
  
  while(matrix.repeatCount() != 3) {    //Wait until picture has been displayed 3 times.
    delay(100);
  }
  
  matrix.picture8Bit(flame, 32, 9, 75, 1, 0); //And same as before, you can make it static by setting number of repeats to zero.
  delay(2000);          //Wait a little.

  matrix.deleteScroll();  //Delete everything that is on the display.

  matrix.message("You can even shutdown drivers to save energy........see?", 75, 1, -1);
  delay(24000);
  matrix.shutdown();
  delay(2000);
  matrix.powerUp();
  delay(6000);
  
  matrix.brightness(25, 1);                   //Set a new brightness.

  /*
      Now, this is cool! Make a function with void as the return type (in this example the function is named 'func1', 
      but you can name it whatever you want) and with no parameters. Inside this function, you can use any function of
      the Adafruit GFX library (except drawPixel, instead use writePixel();) and it will scroll!
  */
  matrix.scrollAll(func1, 100, 1, -1);

} //end of function setup()

void loop() {
  //And all this is happening in the background, so you can do your regular stuff, like writing data to serial. Neat!
  Serial.print("Content has been displayed ");
  Serial.print(matrix.repeatCount(), DEC);
  Serial.println(" times");
  delay(2000);
}

void func1() {
  matrix.writePixel(1, 4, 128);
  matrix.setCursor(3, 0);
  matrix.print("Adafruit GFX functions! :D ");
  matrix.fillRect(160, 1, 5, 4, 15);
  matrix.drawLine(180, 8, 230, 0, 128);
  matrix.setCursor(240, 0);
  matrix.print("...and this is the end!");
}
