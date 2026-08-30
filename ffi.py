# ffi.py
import keys
import ctypes
from pathlib import Path
lib_path = Path(__file__).parent / "a2d.so"
c_engine = ctypes.CDLL(str(lib_path))

# Structures

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

class Texture(ctypes.Structure):
    _fields_ = [
        ("handle", ctypes.c_void_p),
        ("width", ctypes.c_int32),
        ("height", ctypes.c_int32),
        ("loaded", ctypes.c_uint8),
    ]

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

c_engine.engine_key_down.argtypes = [ctypes.c_void_p, ctypes.c_int]
c_engine.engine_key_down.restype = ctypes.c_uint8

c_engine.engine_key_pressed.argtypes = [ctypes.c_void_p, ctypes.c_int]
c_engine.engine_key_pressed.restype = ctypes.c_uint8

c_engine.engine_key_released.argtypes = [ctypes.c_void_p, ctypes.c_int]
c_engine.engine_key_released.restype = ctypes.c_uint8

c_engine.engine_get_mouse_pos.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]
c_engine.engine_get_mouse_pos.restype = None

c_engine.engine_mouse_button_down.argtypes = [ctypes.c_void_p, ctypes.c_int]
c_engine.engine_mouse_button_down.restype = ctypes.c_uint8

c_engine.engine_draw_rect.argtypes = [ctypes.c_void_p, Rect, Color, ctypes.c_uint8]
c_engine.engine_draw_rect.restype = None

c_engine.engine_draw_line.argtypes = [ctypes.c_void_p, Vec2, Vec2, Color]
c_engine.engine_draw_line.restype = None

c_engine.engine_draw_point.argtypes = [ctypes.c_void_p, Vec2, Color]
c_engine.engine_draw_point.restype = None

c_engine.engine_texture_load.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
c_engine.engine_texture_load.restype = Texture

c_engine.engine_texture_destroy.argtypes = [ctypes.POINTER(Texture)]
c_engine.engine_texture_destroy.restype = None

c_engine.engine_draw_texture.argtypes = [ctypes.c_void_p, ctypes.POINTER(Texture), Rect]
c_engine.engine_draw_texture.restype = None

c_engine.engine_font_load.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
c_engine.engine_font_load.restype = ctypes.c_void_p   # opaque Font

c_engine.engine_font_destroy.argtypes = [ctypes.c_void_p]
c_engine.engine_font_destroy.restype = None

c_engine.engine_font_render_text.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_char_p, Color]
c_engine.engine_font_render_text.restype = Texture

c_engine.engine_font_measure_text.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]
c_engine.engine_font_measure_text.restype = None

#  Python High-Level Engine Wrapper

class Atom2D:
    def __init__(self, title: str, width: int, height: int, target_fps: int = 60):
        self._ptr = c_engine.engine_create(title.encode("utf-8"), width, height, target_fps)
        if not self._ptr:
            raise RuntimeError("Failed to initialize engine (check stderr)")

    @property
    def is_running(self) -> bool:
        return bool(c_engine.engine_is_running(self._ptr))

    @property
    def delta_time(self) -> float:
        return c_engine.engine_get_delta_time(self._ptr)

    @property
    def fps(self) -> int:
        return c_engine.engine_get_fps(self._ptr)

    @property
    def mouse_pos(self) -> tuple[int, int]:
        x, y = ctypes.c_int(), ctypes.c_int()
        c_engine.engine_get_mouse_pos(self._ptr, ctypes.byref(x), ctypes.byref(y))
        return x.value, y.value

    # font operations
    def load_font(self, path: str, size: int) -> ctypes.c_void_p:
        font = c_engine.engine_font_load(self._ptr, path.encode("utf-8"), size)
        if not font:
            raise RuntimeError(f"Failed to load font: {path}")
        return font

    def render_text(self, font, text: str, r, g, b, a=255) -> Texture:
        tex = c_engine.engine_font_render_text(self._ptr, font, text.encode("utf-8"), Color(r, g, b, a))
        if not tex.loaded:
            raise RuntimeError(f"Failed to render text: {text!r}")
        return tex

    def destroy_font(self, font):
        c_engine.engine_font_destroy(font)

    def measure_text(self, font, text: str) -> tuple[int, int]:
        w, h = ctypes.c_int(), ctypes.c_int()
        c_engine.engine_font_measure_text(font, text.encode("utf-8"), ctypes.byref(w), ctypes.byref(h))
        return w.value, h.value

    # texture operation
    def load_texture(self, path: str) -> Texture:
        tex = c_engine.engine_texture_load(self._ptr, path.encode("utf-8"))
        if not tex.loaded:
            raise RuntimeError(f"Failed to load texture: {path}")
        return tex

    def draw_texture(self, tex: Texture, x, y, w=None, h=None):
        dest = Rect(x, y, w if w is not None else tex.width, h if h is not None else tex.height)
        c_engine.engine_draw_texture(self._ptr, ctypes.byref(tex), dest)

    def destroy_texture(self, tex: Texture):
        c_engine.engine_texture_destroy(ctypes.byref(tex))

    # draw operations
    def draw_rect(self, x, y, w, h, r, g, b, a=255, filled=True):
        c_engine.engine_draw_rect(self._ptr, Rect(x, y, w, h), Color(r, g, b, a), int(filled))

    def draw_line(self, x1, y1, x2, y2, r, g, b, a=255):
        c_engine.engine_draw_line(self._ptr, Vec2(x1, y1), Vec2(x2, y2), Color(r, g, b, a))

    def draw_point(self, x, y, r, g, b, a=255):
        c_engine.engine_draw_point(self._ptr, Vec2(x, y), Color(r, g, b, a))

    # input operations
    def key_down(self, scancode: int) -> bool:
        return bool(c_engine.engine_key_down(self._ptr, scancode))

    def key_pressed(self, scancode: int) -> bool:
        return bool(c_engine.engine_key_pressed(self._ptr, scancode))

    def key_released(self, scancode: int) -> bool:
        return bool(c_engine.engine_key_released(self._ptr, scancode))

    def mouse_button_down(self, button: int) -> bool:
        return bool(c_engine.engine_mouse_button_down(self._ptr, button))

    # low-level operations
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
    # window size
    WIDTH, HEIGHT = 800, 600

    with Atom2D("Engine", WIDTH, HEIGHT, target_fps=60) as app:
        app.set_clear_color(20, 20, 25, 255)
        x, y = 400.0, 300.0
        speed = 200.0

        # textures
        image = app.load_texture("./resource/texture.jpg")

        # fonts
        label = "ATOM2D"
        fnt = app.load_font("./resource/r_fallouty.ttf", 40)
        fnt_tex = app.render_text(fnt, label, 255, 255, 255)
        w, h = app.measure_text(fnt, label)

        while app.is_running:
            app.begin_frame()
            if app.key_pressed(keys.SCANCODE_ESCAPE):
                print("Escape caught, exiting...")
                break

            # inputs
            dt = app.delta_time
            if app.key_down(keys.SCANCODE_W): y -= speed * dt
            if app.key_down(keys.SCANCODE_S): y += speed * dt
            if app.key_down(keys.SCANCODE_A): x -= speed * dt
            if app.key_down(keys.SCANCODE_D): x += speed * dt

            # frame begin
            app.clear()
            app.draw_rect(x - 25, y - 25, 50, 50, 220, 80, 80)
            app.draw_texture(image, 1, 1, 64, 64)
            app.draw_texture(fnt_tex, (WIDTH - w) / 2, (HEIGHT - h) / 2)
            app.end_frame()
            # frame end

        # clean up
        app.destroy_texture(image)
        app.destroy_texture(fnt_tex)
        app.destroy_font(fnt)
