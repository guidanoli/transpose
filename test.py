import subprocess
import pathlib
import pytest

transpose_path = pathlib.Path(__file__).parent / 'transpose'
if not transpose_path.is_file():
    exit("Could not find file '%s'" % transpose_path)

@pytest.fixture
def all_notes():
    return "A A# Ab B B# Bb C C# Cb D D# Db E E# Eb F F# Fb G G# Gb"

def run(*args, **kwargs):
    return subprocess.run([transpose_path, *args], capture_output=True, encoding='ascii', **kwargs)

def test_no_args():
    info = run()
    assert info.returncode != 0

def test_zero_offset_sharp(all_notes):
    info = run('0', input=all_notes)
    assert info.returncode == 0
    assert info.stdout == "A A# G# B C A# C C# B D D# C# E F D# F F# E G G# F#"

def test_zero_offset_flat(all_notes):
    info = run('0', '-b', input=all_notes)
    assert info.returncode == 0
    assert info.stdout == "A Bb Ab B C Bb C Db B D Eb Db E F Eb F Gb E G Ab Gb"
