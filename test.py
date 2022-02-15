import sys

if sys.platform == 'linux':
    sys.path.append("./.out-linux64")
else:
    sys.path.append("./.out-win64")

import mymodule

foo = mymodule.MyClass()
print(foo, 'add =', foo.add(1,2))