# Golf Game: Graphics Final project

## Video Demonstrating Code
(https://www.youtube.com/watch?v=j5RpLCWdhDQ)

## Description

This simple golf game was created over roughly 15 hours from start to finish. Much of the object-handling code was reused from previous projects. The build requires one library from class which I did not feel was proper to include in this repository.

The most in-depth feature is the collision detection and handling. Each stationary object is shaped in a manner s.t every object has an axis-aligned-bounding box that contains it. All collisions in-game currently involve the ball and some other object. Collision between sphere and axis aligned bounding box is checked for every object. For every collision, a more precise collision detection is checked, if applicable, and then the collision is resolved.

The collision of the ball with the sloped surface is of particular note. I managed to get it to work without needing to perform any trig functions which allows it to run quite nicely. It does a couple of divisions, although these values could be precomputed for each slope if speed was really a concern. Since the slope collision will realistically only once a time step at most, it isn't really necessary. Additionally, this was made for a final project and time was quite limited, so I focused only on the essentials.