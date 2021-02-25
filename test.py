import random
import string
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

class Notes:
    pass

@pytest.mark.parametrize("input, output, args", [
    (None, "", ['1']),
    (Notes(), "A A# G# B C A# C C# B D D# C# E F D# F F# E G G# F#", ['0']),
    (Notes(), "A# B A C C# B C# D C D# E D F F# E F# G F G# A G", ['1']),
    (Notes(), "G# A G A# B A B C A# C# D C D# E D E F D# F# G F", ['-1']),
    (Notes(), "A Bb Ab B C Bb C Db B D Eb Db E F Eb F Gb E G Ab Gb", ['0', '-b']),
    (Notes(), "Bb B A C Db B Db D C Eb E D F Gb E Gb G F Ab A G", ['1', '-b']),
    (Notes(), "Ab A G Bb B A B C Bb Db D C Eb E D E F Eb Gb G F", ['-1', '-b']),
    ("#b# # b bb ##", "#b# # b bb ##", ['1']),
    ("A# A Ab b #", "B A# A b #", ['1']),
    ("A# A Ab b #", "B Bb A b #", ['1', '-b']),
    ("A## B## C## D## E## F## G##", "A# C C# D# F F# G#", ['-1']),
    ("Abb Bbb Cbb Dbb Ebb Fbb Gbb", "Ab Bb B Db Eb E Gb", ['1', '-b']),
    ("A##############", "B", ['0']),
    ("Abbbbbbbbbbbbbb", "G", ['0']),
    ("Ab# A#b Abb# A##b Abb#b A##b#", "A A G# A# G B", ['0']),
])
def test_generic(input, output, args, notes):
    if isinstance(input, Notes):
        input = notes
    info = run(*args, input=input)
    assert info.returncode == 0, info.stderr
    assert info.stdout == output

@pytest.mark.parametrize("note, offset", [
    (note, str(offset)) for note in generate_notes() for offset in range(12)
])
def test_buffer_overflow(note, offset):
    info = run(offset, input=note)
    assert info.returncode == 0, info.stderr
    expected = info.stdout
    bufsize = 2 ** 13
    info = run(offset, input=' '.join(note for _ in range(bufsize)))
    assert info.returncode == 0, info.stderr
    assert info.stdout == ' '.join(expected for _ in range(bufsize))

def test_noise():
    noise = ''.join(random.choice(string.ascii_lowercase) for _ in range(2 ** 13))
    info = run('1', input=noise)
    assert info.returncode == 0, info.stderr
    assert info.stdout == noise
