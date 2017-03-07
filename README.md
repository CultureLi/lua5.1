tolua++ for c++ 
vs-2015 lua5.1 tolua++tolua++-1.0.92

use vs build tolua++.lib tolua++.exe

test code to test c++ and lua call function each other


note:

add tolua++ in   "vs - tools -External tools"

commond: your tolua++.exe direction
args   : -o XXX.cpp XXX.pkg
Initial dir: $(SolutionDir) or change it according to your project dir

use the tool we can get tolua++ export fucntion in XXX.cpp, whitch we can call in lua  
