# GL3 WindowsThis project demonstrates how to use a modern OpenGL 3.0 to display a sphere with some procedurally distorted texture on it.My main goal was to simulate a burning planet.My auxiliary goal was to create an efficient (i.e. GPU-based) implementation using modern OpenGL API. Also, to create relatively simple but still useful framework for making OpenGL-based windows applications.Here's a screenshot:<br/>![Screenshot](screenshot.jpg)<br/>The flame is animated in the real-time. The noise for that is generated completely on the GPU.## Building and RunningTo build, you’ll need Visual Studio 2015 update 3. To run, you only need a Windows PC with GPU and OS supporting OpenGL 3.0 or above.There’re no runtime dependencies. In release builds, CRT is statically linked, and all required assets are embedded into the EXE.Tested on the following systems:* GeForce GTX 960, Windows 10 x64* GeForce GT 540M, Windows 8.1 x64* Intel HD 3000, Windows 8.1 x64 – had to downgrade OpenGL to 3.1 (initially it was 4.0).* Intel HD 4000, Windows 10 x64* VmWare 12, Windows 7 x86 – that was interesting.In the release notes, VmWare wrote “[Advanced graphics, OpenGL 3.3](http://pubs.vmware.com/Release_Notes/en/workstation/12pro/workstation-12-release-notes.html)” but apparently that’s not true.The hardware doesn’t even support OpenGL 3.1 GLSL. Literally, the API said “GLSL 1.40 is not supported. Supported versions are: 1.10, 1.20, 1.30, 1.00 ES, and 3.00 ES”.Fortunately, my shaders were OK with downgrade to GLSL 1.3 that’s part of OpenGL 3.0.Also, I had to implement manual S3 texture decompression. The virtual GPU doesn't support `GLEW_EXT_texture_compression_s3tc` extension.## ControlsLeft mouse drag – rotate the sphere.Up/down keys (or ‘8’ / ‘2’ on numeric keypad) – adjust noise zoom.Left/right keys (or ‘4’ / ‘6’ on numeric keypad) – adjust speed.Plus/minus keys – adjust distortion intensity.Enter or ‘5’ on numeric keypad – reset everything.Alt+Enter – toggle fullscreen.## Pre-Existing IPThis project includes some open source parts.DirectX Tool Kit<br/>(c) Microsoft<br/>MIT License<br/>https://github.com/Microsoft/DirectXTKOpenGL Extension Wrangler Library<br/>Modified BSD License, the Mesa 3-D License (MIT) and the Khronos License (MIT).<br/>https://github.com/nigels-com/glewDXT Decompress<br/>Custom permissive license<br/>https://bitbucket.org/Anteru/dxt-decompressSimplex noise<br/>(c) Ashima Arts<br/>Custom permissive license<br/>https://raw.githubusercontent.com/ashima/webgl-noise/master/LICENSE