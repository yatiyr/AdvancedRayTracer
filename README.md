# AdvancedRayTracer
I made a basic ray tracer in Ceng477. Now it is time to implement an advanced one for Ceng795.


# UPDATE 29th March 0:19

I have initialized main functions of renderer. I can now load a texture from cpu and process it
in GPU using glsl shaders. Here is an example gif, 16x16 work groups are processed in parallel

This gif shows each work group doing its job specified in compute shader.

![alt text](./gifs/computeShaderTest1.gif)

This is a fancier job :D

![alt text](./gifs/computeShaderTest2.gif)

