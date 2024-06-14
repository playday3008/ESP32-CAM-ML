# This directory is intended for project specific (private) libraries

PlatformIO will compile them to static libraries and link into executable file.

The source code of each library should be placed in an own separate directory
`lib/your_library_name/[here are source files]`.

For example, see a structure of the following two libraries `Foo` and `Bar`:

<style>
  pre {
    font-family: monospace;
  }
</style>

<pre>
|--lib
|  |
|  |--Bar
|  |  |--docs
|  |  |--examples
|  |  |--src
|  |  |  |- Bar.c
|  |  |  |- Bar.h
|  |  |- <a href="https://docs.platformio.org/page/librarymanager/config.html">library.json (optional, custom build options, etc)</a>
|  |
|  |--Foo
|  |  |- Foo.c
|  |  |- Foo.h
|  |
|  |- README.md --> THIS FILE
|
|- platformio.ini
|--src
|  |- main.c
</pre>

and a contents of `src/main.c`:

```c
#include <Foo.h>
#include <Bar.h>

int main (void)
{
  ...
}
```

PlatformIO Library Dependency Finder will find automatically dependent
libraries scanning project source files.

[More information about PlatformIO Library Dependency Finder](https://docs.platformio.org/page/librarymanager/ldf.html)
