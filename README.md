# Turtle
Simple turtle graphics program written in C++
---
Pass commands to the turtle through an in-built console
Use directional arrows(up, down) to navigate through your command history

## Usage

Write the command followed by parameters separated by spaces into the console 

Commands: 

* 420 - Blaze it
* rotate x - The turtle rotates by x degrees
* move x - The turtle moves by x in whatever direction it is currently facing
* circle x degree=360 - The turtle draws an arc with diameter x, if you don't want a full circle, pass in parameter degree, which sets the measure of the arc, if degree is a negative number, the arc will be drawn in the opposite direction
* color r g b - Pass in an rgb value to set the color of the brush
* background r g b - Pass in an rgb values to set the color of the background
* thickness x - Set the thickness of the brush to x
* speed x - Set the turtle's speed to x
* penup - Stop drawing
* pendown - Start drawing
* jump x y - Turtle teleports to x, y
* exit - Quit the program

All parameters are signed integers

## Pi constant

You can use pi as a substitution for 180 or -pi as a substitution for -180

Eg. rotate pi

You can use pi as a parameter for any command, since it is just replaced with 180, but it is suggested that you avoid doing that, as that can make your intention unclear

Eg. color pi pi pi

## Example

jump 300 300
pendown
thickness 5
background 234 36 182
move 200
rotate 45
circle 200 180
jump 700 700
circle 250 180
circle 250 -180
420
