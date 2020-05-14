# NipKow-Disc
A recreation of an early 20th century electro-mechanical design for a television. Technology is replicated and further augmented with a video-graphics engine and a falling-bottle game implementation. 



The Nipkow Disc works by placing a disc with spiraled holes on a motor. As the disc spins, the holes pass in front of an LED one at a time. Each hole acts as a row in the final image. This LED modulates its brightness for each pixel. Due to persistance of vision, each full rotation of the disc creates an image.

![NipkowDisc Diagram]("./docs/diagram.png")

Nipkow disc implementation was heavily inspired by https://create.arduino.cc/projecthub/christopheArduino/nipkow-disk-32-line-television-507ec4. The disc has an additional "sync hole" close to its center. When this hole passes between an IR diode and sensor, the sensor sends a signal to the controller, which informs the LED's next pixel to draw. This ensures that the first pixel is in the top-left corner of the image. This sync hole also allows the controller to track the rate of rotation of the motor, and thus update its modulation rate accordingly.

This particular implementation is unique because it has incorporated a graphics engine and game implementation. The graphics engine allows a programmer to draw rectangular "sprites" to the screen and update such sprites with each rotation of the disc. This graphics engine was then utilized to make a "falling droplets" game, in which the user is tasked to collect 16 falling droplets in a row by turning a potentiometer. 
