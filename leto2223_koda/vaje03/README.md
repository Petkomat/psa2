# Rezultati

Dodati ob priliki

# Struktura

Poskusili smo slediti _lepemu_ načinu implementacije:

1. Program poganjamo v `main.cpp`.
2. V `tabele.h` so le deklaracije.
3. V `tabele.cpp` so potrebne implemenetacije.

Ker je treba _skompajlati_ več kot le en `.cpp`, je v `.vscode/tasks.json` ukaz za _kompajler_ malo drugačen:

```
{
    "type": "cppbuild",
    "label": "C/C++: g++.exe build active file",
    "command": "C:\\msys64\\mingw64\\bin\\g++.exe",
    "args": [
        "-fdiagnostics-color=always",
        "-static-libstdc++",
        "-g",
        "${fileDirname}\\**.cpp",
        "-o",
        "${fileDirname}\\${fileBasenameNoExtension}.exe"
    ],
```

Poberemo torej vse dostopne `.cpp` datoteke in jih zapakiramo v `<ime trenutne>.exe` (poženemo na `main.cpp`, tj. ime trenutne `main`)


# Opombe

1. Več o [dedovanju](https://www.geeksforgeeks.org/inheritance-in-c/)
2. Več o [abstraktnih metodah](https://www.geeksforgeeks.org/pure-virtual-functions-and-abstract-classes/) (ang. _pure virtual_) 
3. Destruktorje (četudi abstraktne) je treba [**vedno implementirati**](https://stackoverflow.com/a/12574407/2281318).
