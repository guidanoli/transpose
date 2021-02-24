import subprocess
import pathlib
import pytest

transpose_path = pathlib.Path(__file__).parent / 'transpose'
if not transpose_path.is_file():
    exit("Could not find file '%s'" % transpose_path)

def run(*args, **kwargs):
    return subprocess.run([transpose_path, *args], capture_output=True, encoding='ascii', **kwargs)

def test_no_args():
    info = run()
    assert info.returncode != 0

def test_help():
    info = run('-h')
    assert info.returncode == 0

def generate_notes():
    for letter in "ABCDEFG":
        yield letter
        for accident in "#b":
            yield letter + accident

@pytest.fixture
def notes():
    return ' '.join(generate_notes())

@pytest.fixture
def asnp_notes():
    return '4 '.join(generate_notes()) + '4'

@pytest.mark.parametrize("output, args", [
    ("A A# G# B C A# C C# B D D# C# E F D# F F# E G G# F#", ['0']),
    ("A# B A C C# B C# D C D# E D F F# E F# G F G# A G",    ['1']),
    ("G# A G A# B A B C A# C# D C D# E D E F D# F# G F",    ['-1']),
    ("A Bb Ab B C Bb C Db B D Eb Db E F Eb F Gb E G Ab Gb", ['0', '-b']),
    ("Bb B A C Db B Db D C Eb E D F Gb E Gb G F Ab A G",    ['1', '-b']),
    ("Ab A G Bb B A B C Bb Db D C Eb E D E F Eb Gb G F",    ['-1', '-b']),
])
def test_notes(notes, output, args):
    info = run(*args, input=notes)
    assert info.returncode == 0, info.stderr
    assert info.stdout == output

@pytest.mark.parametrize("output, args", [
    ("A4 A#4 G#4 B4 C4 A#4 C4 C#4 B4 D4 D#4 C#4 E4 F4 D#4 F4 F#4 E4 G4 G#4 F#4", ['0']),
    ("A#4 B4 A4 C5 C#4 B4 C#4 D4 C5 D#4 E4 D4 F4 F#4 E4 F#4 G4 F4 G#4 A4 G4",    ['1']),
    ("G#4 A4 G4 A#4 B3 A4 B3 C4 A#4 C#4 D4 C4 D#4 E4 D4 E4 F4 D#4 F#4 G4 F4",    ['-1']),
    ("A4 Bb4 Ab4 B4 C4 Bb4 C4 Db4 B4 D4 Eb4 Db4 E4 F4 Eb4 F4 Gb4 E4 G4 Ab4 Gb4", ['0', '-b']),
    ("Bb4 B4 A4 C5 Db4 B4 Db4 D4 C5 Eb4 E4 D4 F4 Gb4 E4 Gb4 G4 F4 Ab4 A4 G4",    ['1', '-b']),
    ("Ab4 A4 G4 Bb4 B3 A4 B3 C4 Bb4 Db4 D4 C4 Eb4 E4 D4 E4 F4 Eb4 Gb4 G4 F4",    ['-1', '-b']),
])
def test_asnp_notes(asnp_notes, output, args):
    info = run(*args, '-s', input=asnp_notes)
    assert info.returncode == 0, info.stderr
    assert info.stdout == output

