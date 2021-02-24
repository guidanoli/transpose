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
def bufsiz():
    info = run('-l')
    assert info.returncode == 0, info.stdef
    return int(info.stdout)

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

def test_buffer_overflow(bufsiz):
    for note in generate_notes():
        for offset in range(-11,12):
            info = run(str(offset), input=note)
            assert info.returncode == 0
            expected = info.stdout
            n = bufsiz // len(note)
            info = run(str(offset), input=note*n)
            assert info.returncode == 0
            assert info.stdout == expected*n, (note, offset)

