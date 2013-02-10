For license information, please see LICENSE.txt .  It's the standard M.I.T. License, so you can do pretty much anything you'd want, so long as you don't sue me and do include my license information somewhere if you use my code in something.

If any of my code doesn't compile on some platform or compiler, please let me know and I'll try my best to fix it!

To set up SDL (Simple DirectMedia Layer), required by the code in this repository:

1) Download the "Development Libraries" for your platform from near the bottom of http://www.libsdl.org/download-1.2.php .  This includes the runtime libraries, so you don't need to download them separately.  NOTE: Although they label the Windows versions as "Win32", it contains both 32-bit and 64-bit libraries.

2) Extract the archive somewhere logical (Windows) / install the RPM (Linux)

3) Add the "include" directory to your project include directories (in VisualStudio, it's under Project > Properties > Configuration Properties > C/C++ > General > Additional Include Directories), or if you're using a makefile, add it to your makefile, however your particular brand of makefile requires.

4) Add the SDL.lib file to your linker input (in VisualStudio, it's under Project > Properties > Configuration Properties > Linker > Input > Additional Dependencies), or if you're using a makefile, add it to your makefile, however your particular brand of makefile requires.  NOTE: Use lib/x64/SDL.lib if you're developing a 64-bit application, and use lib/x86/SDL.lib if you're developing a 32-bit application, but then, why are you developing a 32-bit application in modern times?

5) (Windows) Either copy SDL.dll to the directory that your executable will be running in, or put it in the Windows directory, so that it can be found by the executable.  NOTE: If you put lib/x86/SDL.dll in the Windows directory and try to use SDL in a 64-bit application, it might not work, so it's probably safest to just copy it to where your executable will be running.

Why am I making you download SDL yourself, instead of simply including it in this repository like would be logical?  SDL is licensed under LGPL, so it's okay to link against it without incurring the wrath of the GPL, but unfortunately, it's currently ambiguous as to whether I would be allowed to have it in this repository without restricting your use of my code with the GPL.  I want you to be able to do as much as you want and not have to worry about legal nonsense, so I've chosen to leave it out.  Since you'll just be linking against SDL, and not modifying it or using bits of its source code, you won't face legal restrictions you otherwise might or might not have faced.
