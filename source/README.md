# Computer Graphics Assignment(0)

This assignment is a part of the Computer Graphics Elective at IIITH

## How to run

The repo have the following structure

<b>./source</b>
 - main.cpp
 - stb_image.h
 - wall.jpg

The code sample has the following dependencies:

1. GLAD
2. GLFW
3. GLEW
4. GLM

## Running Instructions:

The application prompts for selecting one out of the three available models to be displayed.

1. Hexagonal dipyramid(Colored)
2. Unidecagonal Prism(Texture)
3. Elongated Square dipyramid(Colored)

Enter one of the above indexes, to select a model.

## Buttons:

<b>CAMERA MOVEMENT <br></b>

<button>W</button> - Up <br>
<button>S</button> - Down <br>
<button>A</button> - Left <br>
<button>D</button> - Right <br>
<button>Z</button> - Back <br>
<button>X</button> - Forward <br>

<button>C</button> - Camera translates to Predefined Position 1 and faces object<br>
<button>V</button> - Camera translates to Predefined Position 2 and faces object<br>
<button>B</button> - Camera translates to Predefined Position 3 and faces object<br>


<button>R</button> - Camera starts to Spins about object  <br>
<button>T</button> - Camera stops to Spins about object  <br>

<b>OBJECT MOVEMENT <br></b>

<button>U</button> - Up <br>
<button>J</button> - Down <br>
<button>H</button> - Left <br>
<button>K</button> - Right <br>
<button>N</button> - Back <br>
<button>M</button> - Forward <br>

<button>F</button> - Objects starts to Rotate about X axis  <br>
<button>G</button> - Objects stops to Rotate<br>


## Note:
1. Translation of Object and Camera when, the camera is revolving around the object, will change the radii of revolution

