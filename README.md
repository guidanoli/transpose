# transpose

Transposes musical notes by offset. Example:

```sh
$ cat notes
A A# B C C# D D# E F F# G G#
$ cat notes | transpose -1
G# A A# B C C# D D# E F F# G
$ cat notes | transpose -1 -b
Ab A Bb B C Db D Eb E F Gb G
```

## Installation:

```sh
make               # Compiles
sudo make install  # Installs
```
