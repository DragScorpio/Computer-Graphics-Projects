# Computer-Graphics-Projects
Introduction to Computer Graphics with an emphasis on rendering animation with OpenGL

1. Brain Structure Project

Objective

The objective of this homework is to visualize a brain structure from a provided data file. The data file has a clear format indicating XYZ-coordinates of each vertices, as well as referencing vertices of each triangular facets that serve as basic units of the brain structure. Another goal of this homework is to practice using normal vector to each triangular surface in assist to create light source and nice effect on the visualized brain structure.
Programming Procedure
The code to realize visualization has a single file “main.c”. I firstly defined ambience vector “light_diffuse[]” and “light position[]” in order to creating an infinite light source at XY-plane with diffused red light scattered onto the brain structure. The reason for my choice was to mimic real human organ in surgeon.
In order to store triangle data from reading file, I created two structures “struct vertex” and “struct triangle”. Particularly, these two structures have all the coordinates of vertices ready for calculation use. The “CalculateNormalVector()” function is the one using triangular vertices to calculate each normal vector of a triangle plane, and specify the normal vector with “glNormal3fv()” function.
Finally, I added keyboard control to the code in order to let user switch views of the visualization result by pressing keys. This can be achieved by adding transformation matrices with certain controllable variables. By now, the code can rotate the visualization along X, Y, or Z-axis. It can also zoom in or zoom out the graph. Of course, user can press “r” or “R” to return to the default settings.
Visualization Result
The effect of code execution is satisfying. The following figures illustrate partial and whole part of brain structure from different angles.
  
Conclusion
I learned the use of openGL library functions as well as utility functions in creating 3D-visualized result. Specifically, creating a mimicking visualization result in each industries is a desirable tendency. Through this coding experience, I stepped further into the field of scientific visualization.

2. Robotic Animation Project

In this project, I augment the robot.c used in the OpenGL Programming Guide. In particular, I added two fingers to the robotic arm developed in that example.
  1)The two fingers should look something like the two fingers diplayed here.
  2)The two fingers should pivot at a point near where they join the lower arm, as if they were pinching something.
  3)By pressing the keyboard letter 'O' the fingers should open.
  4)By pressing the keyboard letter 'C' the fingers should close.
  5)Experiment with the elbow and finger angles to determine reasonable maximum and minimum joint angles. Implement a hard coded maximum and minimum joint angles for these joints.
