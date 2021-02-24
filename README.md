# transpose

Transposes musical notes by offset. Example:

```sh
$ cat notes
A A# B C C# D D# E F F# G G#
$ cat notes | transpose -1
G# A A# B C C# D D# E F F# G
$ cat notes | transpose -1 -b
Ab A Bb B C Db D Eb E F Gb G
$ cat notes | transpose 0 -b
A Bb B C Db D Eb E F Gb G Ab
```

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

## Usage

For usage information, run `transpose -h`.
