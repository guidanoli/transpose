# transpose

Transposes musical notes by offset

```sh
$ cat notes
A A# B C C# D D# E F F# G G#
$ cat notes | transpose -1
G# A A# B C C# D D# E F F# G
```

## `-b`

Print with flats instead of sharps.

```sh
$ cat notes | transpose -1 -b
Ab A Bb B C Db D Eb E F Gb G
```

## `-s`

Read notes in scientific pitch notation.

```sh
$ cat notes-spn
A1 A#1 B1 C1 C#1 D1 D#1 E1 F1 F#1 G1 G#1
$ cat notes-spn | tranpose -6 -s
D#1 E1 F1 F#0 G0 G#0 A0 A#0 B0 C1 C#1 D1
```

For more usage information, run `transpose -h`.

## Setup

```sh
make               # Compiles
sudo make install  # Installs
```

## Tests

Running the tests requires Python >= 3.7 and pytest.

```sh
python -m pip install pytest
python -m pytest test.py
```
