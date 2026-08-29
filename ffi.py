import ctypes
from pathlib import Path

lib_path = Path(__file__).parent / "a2d.so"
c_engine = ctypes.CDLL(str(lib_path))

# FFI Mappings rev 0.1:
# - Only value types cross the boundary as Structures.
# - Engine is opaque: struct engine is hidden in engine.c, always a bare c_void_p.

class Color(ctypes.Structure):
    _fields_ = [
        ("r", ctypes.c_uint8),
        ("g", ctypes.c_uint8),
        ("b", ctypes.c_uint8),
        ("a", ctypes.c_uint8),
    ]

class Vec2(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float), ("y", ctypes.c_float)]

class Rect(ctypes.Structure):
    _fields_ = [("x", ctypes.c_float), ("y", ctypes.c_float),
                ("w", ctypes.c_float), ("h", ctypes.c_float)]

#  Function Signatures

c_engine.engine_create.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_uint32]
c_engine.engine_create.restype = ctypes.c_void_p

c_engine.engine_destroy.argtypes = [ctypes.c_void_p]
c_engine.engine_destroy.restype = None

c_engine.engine_begin_frame.argtypes = [ctypes.c_void_p]
c_engine.engine_begin_frame.restype = None

c_engine.engine_clear.argtypes = [ctypes.c_void_p]
c_engine.engine_clear.restype = None

c_engine.engine_end_frame.argtypes = [ctypes.c_void_p]
c_engine.engine_end_frame.restype = None

c_engine.engine_get_delta_time.argtypes = [ctypes.c_void_p]
c_engine.engine_get_delta_time.restype = ctypes.c_float

c_engine.engine_get_fps.argtypes = [ctypes.c_void_p]
c_engine.engine_get_fps.restype = ctypes.c_uint32

c_engine.engine_is_running.argtypes = [ctypes.c_void_p]
c_engine.engine_is_running.restype = ctypes.c_uint8

c_engine.engine_set_clear_color.argtypes = [ctypes.c_void_p, Color]
c_engine.engine_set_clear_color.restype = None

#  Python High-Level Engine Wrapper

class Atom2D:
    def __init__(self, title: str, width: int, height: int, target_fps: int = 60):
        self._ptr = c_engine.engine_create(title.encode("utf-8"), width, height, target_fps)
        if not self._ptr:
            raise RuntimeError("Failed to initialize engine (see stderr for SDL error)")

    @property
    def is_running(self) -> bool:
        return bool(c_engine.engine_is_running(self._ptr))

    @property
    def delta_time(self) -> float:
        return c_engine.engine_get_delta_time(self._ptr)

    @property
    def fps(self) -> int:
        return c_engine.engine_get_fps(self._ptr)

    def set_clear_color(self, r: int, g: int, b: int, a: int = 255):
        c_engine.engine_set_clear_color(self._ptr, Color(r, g, b, a))

    def begin_frame(self):
        c_engine.engine_begin_frame(self._ptr)

    def clear(self):
        c_engine.engine_clear(self._ptr)

    def end_frame(self):
        c_engine.engine_end_frame(self._ptr)

    def close(self):
        if self._ptr:
            c_engine.engine_destroy(self._ptr)
            self._ptr = None

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()


if __name__ == "__main__":
    with Atom2D("Atom2D Engine", 800, 600, target_fps=60) as app:
        app.set_clear_color(30, 20, 50, 255)
        while app.is_running:
            app.begin_frame()   # polls events + computes delta_time internally
            app.clear()
            # draw calls go here later
            app.end_frame()     # presents + fps-caps internally
