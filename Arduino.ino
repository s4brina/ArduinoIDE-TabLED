#include <FastLED.h>

#define NUM_LEDS 241
#define DATA_PIN 7

CRGB leds[NUM_LEDS];


void setup() {
    Serial.begin(9600);
    FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++) {  // Simple test pattern
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(10);
    }
    FastLED.clear();
    FastLED.show();
    Serial.println("LED Setup Complete");
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        Serial.print("Received command: ");
        Serial.println(command);
{

        } if (command == "S") {
            playSparkleAnimation();
        } else if (command == "RAINBOW") {
            playRainbowAnimation();
        } else if (command == "CHILL") {
            playChillAnimation();
        } else if (command == "SUNSET") {
            playSunsetAnimation(); // Start sunset animation
        } else if (command == "BREATHE") {
            playBreathingEffect();
        } else if (command == "RIPPLE") {
            playRippleEffect(); // Start ripple animation
        } else if (command == "COMET") {
            playCometEffect(); // Start comet animation
        } else if (command == "CONFETTI") {
            confetti(); // Start confetti animation
        } else if (command == "STOP") {
            stopAnimations();
        } else if (command == "RESET") {
            resetLEDs();
        } else if (command.startsWith("AUDIO:")) {
            handleAudio(command);
        } else if (command.startsWith("COLOR:")) {
            handleColorCommand(command);
        } else {
            parseLEDCommand(command);
        }
    }
}



// Function to parse individual LED color commands like "53:255:255:255"
void parseLEDCommand(String command) {
    int firstColonIndex = command.indexOf(':');
    if (firstColonIndex != -1) {
        int ledNumber = command.substring(0, firstColonIndex).toInt();
        command = command.substring(firstColonIndex + 1);

        int secondColonIndex = command.indexOf(':');
        int r = command.substring(0, secondColonIndex).toInt();
        command = command.substring(secondColonIndex + 1);

        int thirdColonIndex = command.indexOf(':');
        int g = command.substring(0, thirdColonIndex).toInt();
        int b = command.substring(thirdColonIndex + 1).toInt();

        if (ledNumber >= 1 && ledNumber <= NUM_LEDS) {
            int snakeIndex = getSnakeIndex(ledNumber);
            leds[snakeIndex - 1] = CRGB(constrain(r, 0, 255), constrain(g, 0, 255), constrain(b, 0, 255));
            FastLED.show();
            Serial.print("Set LED ");
            Serial.print(ledNumber);
            Serial.println(" color updated");
        } else {
            Serial.println("Invalid LED number");
        }
    }
}



void handleColorCommand(String command) {
    int firstComma = command.indexOf(':') + 1;
    int secondComma = command.indexOf(',', firstComma);
    int thirdComma = command.indexOf(',', secondComma + 1);

    int r = constrain(command.substring(firstComma, secondComma).toInt(), 0, 255);
    int g = constrain(command.substring(secondComma + 1, thirdComma).toInt(), 0, 255);
    int b = constrain(command.substring(thirdComma + 1).toInt(), 0, 255);

    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    Serial.println("All LEDs set to color");
}

// Reset all LEDs to off
void resetLEDs() {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    Serial.println("All LEDs reset");
}

// Simple audio detection example
void handleAudio(String command) {
    int audioLevel = command.substring(6).toInt();
    if (audioLevel > 50) {
        fill_solid(leds, NUM_LEDS, CRGB::Green);
    } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black); // Clear instead of FastLED.clear()
    }
    FastLED.show();
    Serial.print("Audio level set LEDs");
}

// Fade In and Out function
void fadeInOut() {
    for (int brightness = 0; brightness <= 255; brightness++) {
        fill_solid(leds, NUM_LEDS, CRGB(brightness, brightness, brightness));
        FastLED.show();
        delay(10);
    }
    for (int brightness = 255; brightness >= 0; brightness--) {
        fill_solid(leds, NUM_LEDS, CRGB(brightness, brightness, brightness));
        FastLED.show();
        delay(10);
    }
}

// Chase Effect animation
void chaseEffect() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(50);
        leds[i] = CRGB::Black;
    }
}

// Stop animations
void stopAnimations() {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    Serial.println("Animations stopped and LEDs turned off");
}
int getSnakeIndex(int ledNumber) {
    int columns = 20;
    int rows = (NUM_LEDS - 1) / columns;

    int row = (ledNumber - 1) / columns;
    int col = (ledNumber - 1) % columns;

    if (row % 2 == 1) {
        col = columns - 1 - col;
    }

    return (row * columns) + col + 2;
}

#define MAX_SPARKLES 50 // Maximum number of sparkles to show at once
#define NUM_SPARKLES 20  // Number of sparkles to show at a time
#define FADE_SPEED 1000   // Speed of fade-out effect
#define SPARKLE_DELAY 50 // Delay between updates


void playSparkleAnimation() {
    Serial.println("Playing sparkle animation");

    while (true) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        for (int i = 0; i < NUM_SPARKLES; i++) {
            int pos = random(NUM_LEDS);
            int sparkleBrightness = random(128, 255);
            leds[pos] = CHSV(random(0, 255), 255, sparkleBrightness);
        }
        FastLED.show();
        delay(SPARKLE_DELAY);
    }
}

void playRainbowAnimation() {
      static uint8_t colorIndex = 0;

    // Gradually change the color
    for (int i = 0; i < NUM_LEDS; i++) {
        // Create a smooth transition through colors
        leds[i] = CHSV(colorIndex + (i * (255 / NUM_LEDS)), 255, 255);
    }

    FastLED.show(); // Update the LEDs
    colorIndex += 2; // Adjust the speed of the color transition
    if (colorIndex >= 255) {
        colorIndex = 0; // Reset color index for continuous looping
    }

    delay(30); // Adjust delay for smoothness
}

void playChillAnimation() {
  // Define the colors for the fade
  CRGB green = CRGB(0, 255, 0);
  CRGB blue = CRGB(0, 0, 255);
  CRGB purple = CRGB(128, 0, 128);
  
  // Fade from green to blue
  for (int i = 0; i <= 255; i++) {
    CRGB currentColor = blend(green, blue, i);
    fill_solid(leds, NUM_LEDS, currentColor);
    FastLED.show();
    delay(10); // Adjust delay for speed of transition
  }
  
  // Fade from blue to purple
  for (int i = 0; i <= 255; i++) {
    CRGB currentColor = blend(blue, purple, i);
    fill_solid(leds, NUM_LEDS, currentColor);
    FastLED.show();
    delay(10);
  }

  // Fade from purple to green
  for (int i = 0; i <= 255; i++) {
    CRGB currentColor = blend(purple, green, i);
    fill_solid(leds, NUM_LEDS, currentColor);
    FastLED.show();
    delay(10);
  }
}

#define LED_WIDTH   20
#define LED_HEIGHT  12

void playSunsetAnimation() {
    while (true) { // Run indefinitely until reset
        for (int wave = 0; wave < LED_WIDTH; wave++) {
            for (int y = 0; y < LED_HEIGHT; y++) {
                int index = (y * LED_WIDTH + (wave % LED_WIDTH)) % NUM_LEDS;
                leds[index] = CHSV((wave * 10) % 255, 255, 255); // Color cycling
            }
            FastLED.show();
            delay(50);
            
            // Clear LEDs after each wave
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CRGB::Black;
            }
        }
    }
}

void playBreathingEffect() {
   while (true) { // Run indefinitely until reset
        for (int brightness = 0; brightness < 256; brightness++) {
            fill_solid(leds, NUM_LEDS, CRGB::Purple);
            FastLED.setBrightness(brightness);
            FastLED.show();
            delay(10); // Adjust this value for speed of brightness change
        }
        for (int brightness = 255; brightness >= 0; brightness--) {
            fill_solid(leds, NUM_LEDS, CRGB::Purple);
            FastLED.setBrightness(brightness);
            FastLED.show();
            delay(10); // Adjust this value for speed of brightness change
        }
    }
}

void playRippleEffect() {
    int centerX = LED_WIDTH / 2;
    int centerY = LED_HEIGHT / 2;
    int maxRadius = max(centerX, centerY); // Maximum radius

    while (true) { // Loop indefinitely
        // Outward ripple
        for (int radius = 0; radius <= maxRadius; radius++) {
            for (int angle = 0; angle < 360; angle += 5) {
                int x = centerX + radius * cos(angle * DEG_TO_RAD);
                int y = centerY + radius * sin(angle * DEG_TO_RAD);
                if (x >= 0 && x < LED_WIDTH && y >= 0 && y < LED_HEIGHT) {
                    int index = (y * LED_WIDTH + x) % NUM_LEDS;
                    leds[index] = CRGB::Cyan; // Color for the ripple
                }
            }
            FastLED.show();
            delay(100); // Delay to slow down the effect
            
            // Clear LEDs after displaying the current radius
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CRGB::Black;
            }
        }

        // Inward ripple
        for (int radius = maxRadius; radius >= 0; radius--) {
            for (int angle = 0; angle < 360; angle += 5) {
                int x = centerX + radius * cos(angle * DEG_TO_RAD);
                int y = centerY + radius * sin(angle * DEG_TO_RAD);
                if (x >= 0 && x < LED_WIDTH && y >= 0 && y < LED_HEIGHT) {
                    int index = (y * LED_WIDTH + x) % NUM_LEDS;
                    leds[index] = CRGB::Cyan; // Color for the ripple
                }
            }
            FastLED.show();
            delay(100); // Delay to slow down the effect
            
            // Clear LEDs after displaying the current radius
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CRGB::Black;
            }
        }
    }
}
void playCometEffect() {
    // Initial delay to prevent the comet from starting halfway
    for (int init = 0; init < NUM_LEDS; init++) {
        leds[init] = CRGB::Black; // Make sure all LEDs are off initially
    }
    FastLED.show();
    delay(500); // Initial delay before starting the effect

    while (true) { // Loop indefinitely
        for (int i = 0; i < NUM_LEDS + LED_WIDTH; i++) {
            // Clear LEDs
            for (int j = 0; j < NUM_LEDS; j++) {
                leds[j] = CRGB::Black;
            }

            // Create a comet trail
            for (int j = 0; j < LED_WIDTH; j++) {
                int index = (i - j + NUM_LEDS) % NUM_LEDS; // Wrap around
                if (j < 3) {
                    leds[index] = CRGB::White; // Bright color for the comet head
                } else if (j < 10) {
                    leds[index] = CRGB::Blue; // Fading trail
                }
            }

            FastLED.show();
            delay(30); // Adjust the speed of the comet
        }
    }
}

#define STROBE_ON_DELAY 700 // Delay for the "on" part of the strobe
#define STROBE_OFF_DELAY 20 // Shorter delay for the "off" part of the strobe
#define COLOR_CHANGE_DELAY 150  // Faster delay for color change
void confetti() {
  CRGB colors[] = { CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Purple, CRGB::Yellow, CRGB::Cyan, CRGB::Magenta };
  int colorIndex = 0;

  // Infinite loop within the function
  while (true) {
    // Set LEDs to the current color for a strobe effect
    fill_solid(leds, NUM_LEDS, colors[colorIndex]);
    FastLED.show();
    delay(STROBE_ON_DELAY);

    // Briefly turn LEDs off for the strobe "flash" effect
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    delay(STROBE_OFF_DELAY);

    // Cycle to the next color
    colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
  }
}



void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    FastLED.clear(); // Clear previous LED states
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(red, green, blue); // Set new color
    }
    FastLED.show();
}
