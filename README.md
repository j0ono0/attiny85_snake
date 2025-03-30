# Snake running on an custom built ATtiny85 game console 

Snake game that runs on a custom designed and built handheld game console. At its heart is a ATtiny85 microcontroller. This project has been my first attempt using surface mount devices and my second attempt at designing a PCB.

I'm in the process of a complete write up. Feel free to ask question via social media if this project is of interest to you.

Snake has been written in C. This provides good control over program size and performance which is critical due to the restricted hardware specifications.


## Hardware

Initial PCB design was (unsurprisingly) dictated by the format a handheld game console might take. A few layouts were considered, finally settling on a landsacpe composition that would provide a more comfortable device to use plus create a little more space for parts. Maximum dimensions were set to fit within 10cm x 10cm. This matches a cost effective price-point offered by PCBWay, the PCB manufacturer I selected for this project. With primary layout goals loosly in place, component selection and exact positioning was undertaken. Component selection was dictated by what I though (with no prior experience) I could successfully solder by hand with a soldering iron. On the PCB this translated to footprints with larget pads and generous spacing between components.

### Attiny85 microcontroller

 - 8kb program memory
 - 512b ram
 - pin count: 8

### OLED Display

- ssd1306 driver
- I2C communication
- 128 x 64 pixel resolution
- monochrome

### Audio

- piezo speaker

Chip tunes only! Audio is controlled by a timer and interrupts on the microcontroller.

### Input

- x4 buttons 'D-pad'
- x2 buttons 'actions'

6 buttons are divided into 2 groups. Each group is monitored by its own dedicated ADC enabled pin on the microcontroller. Buttons are assembled with resistors to create a voltage divider circuit allowing multiple buttons to be read on a single pin.

It would be possible to read concurrent button presses via a single pin, but testing suggested splitting d-pad and action button groups would allow faster detection of single and concurrent button presses. For a gaming platorm minmising input delay is critical for a good user experience.


## Firmware

The programming logic is only a portion of the required code Snake game is relatively trivial
- i2c communication protocol
- display driver
- audio driver
- button monitoring

## Enclosure design

Initially the hardware was screwed to a blank circuit board with spacers between them. This allowed space for a battery to be secured 'internally', gave some basic protection to components, and made the device comfortable to hold and use.

Whilst this solution fundamentally worked, I decided to design an enclosure that would provide improved protection and refine the visual appearance. The design has been kept minimal with a focus on function but keeping the upper side 'raw' to keep a reference the home-lab designed and hand-assembled origins of the project.
