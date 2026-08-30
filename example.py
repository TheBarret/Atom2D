"""
Atom2D Scene Sample

"""

import math
import keys
from ffi import Atom2D, Vec2, Rect, Line2D, Ray2D, Helper

WIDTH, HEIGHT = 800, 600

#  Rotating polygon setup
# Irregular radii per vertex => "uneven" shape, not a regular hexagon.
POLY_CENTER = Vec2(560.0, 320.0)
POLY_RADII = [40.0, 65.0, 35.0, 70.0, 45.0, 58.0]
POLY_SIDES = len(POLY_RADII)

BASE_VERTS = [
    Vec2(POLY_RADII[i] * math.cos(2 * math.pi * i / POLY_SIDES),
         POLY_RADII[i] * math.sin(2 * math.pi * i / POLY_SIDES))
    for i in range(POLY_SIDES)
]

#  Sensor beam (fixed raycast, independent of player/mouse)
BEAM_Y = 320.0
BEAM_ORIGIN = Vec2(0.0, BEAM_Y)
BEAM_DIR = Vec2(1.0, 0.0)  # already unit length, no normalize needed
BEAM_MAX_DIST = float(WIDTH)

#  Primitives gallery (top-left)
GALLERY_SWATCH = Rect(20.0, 60.0, 60.0, 40.0)


def main():
    with Atom2D("Atom2D Example Scene", WIDTH, HEIGHT, target_fps=60) as app:
        app.set_clear_color(18, 18, 24, 255)

        #  deliberate failure, to show last_error works -
        try:
            app.load_texture("./resource/does_not_exist.png")
        except RuntimeError as e:
            print(f"[expected] last_error={e}")

        #  real assets
        logo = app.load_texture("./resource/texture.jpg")

        title_font = app.load_font("./resource/r_fallouty.ttf", 40)
        # separate load: size is baked in at load time
        debug_font = app.load_font("./resource/r_fallouty.ttf", 15)

        title_text = "ATOM2D"
        title_tex = app.render_text(title_font, title_text, 255, 255, 255)
        title_w, title_h = app.measure_text(title_font, title_text)

        gallery_label = app.render_text(debug_font, "Primitives", 200, 200, 200)

        rotation = 0.0
        paused = False
        debug_tex = None  # holds the current HUD texture; must be destroyed before replacement

        while app.is_running:
            app.begin_frame()

            if app.key_pressed(keys.SCANCODE_ESCAPE):
                print("Escape caught, exiting...")
                break
            if app.key_pressed(keys.SCANCODE_SPACE):
                paused = not paused

            dt = app.delta_time
            if not paused:
                rotation += dt * 1.2  # radians/sec

            app.clear()

            #  rotating uneven polygon: rebuild world-space verts
            world_verts = [POLY_CENTER + v.rotate(rotation) for v in BASE_VERTS]
            edges = [
                Line2D(world_verts[i], world_verts[(i + 1) % POLY_SIDES])
                for i in range(POLY_SIDES)
            ]

            for edge in edges:
                app.draw_line(edge.p1.x, edge.p1.y, edge.p2.x, edge.p2.y, 90, 200, 220)
            for v in world_verts:
                app.draw_point(v.x, v.y, 255, 255, 255)

            #  sensor beam vs polygon edges: nearest hit wins
            best_hit = None
            for edge in edges:
                hit = Helper.raycast_line(Ray2D(BEAM_ORIGIN, BEAM_DIR), edge, BEAM_MAX_DIST)
                if hit.hit and (best_hit is None or hit.distance < best_hit.distance):
                    best_hit = hit

            if best_hit is not None:
                app.draw_line(BEAM_ORIGIN.x, BEAM_ORIGIN.y, best_hit.point.x, best_hit.point.y, 255, 60, 60)
                app.draw_rect(best_hit.point.x - 4, best_hit.point.y - 4, 8, 8, 60, 255, 90)
            else:
                end_x = BEAM_ORIGIN.x + BEAM_DIR.x * BEAM_MAX_DIST
                app.draw_line(BEAM_ORIGIN.x, BEAM_ORIGIN.y, end_x, BEAM_ORIGIN.y, 255, 220, 60)

            #  primitives gallery, with mouse-hover collision check
            mouse_x, mouse_y = app.mouse_pos
            mouse_vec = Vec2(float(mouse_x), float(mouse_y))
            hovering = Helper.check_point_in_rect(mouse_vec, GALLERY_SWATCH)
            swatch_color = (255, 210, 90) if hovering else (120, 120, 130)
            app.draw_rect(GALLERY_SWATCH.x, GALLERY_SWATCH.y, GALLERY_SWATCH.w, GALLERY_SWATCH.h, *swatch_color)
            app.draw_rect(20, 115, 60, 40, 90, 160, 255, filled=False)
            app.draw_line(20, 175, 80, 205, 200, 90, 220)
            for i in range(5):
                app.draw_point(20 + i * 12, 220, 255, 255, 255)
            app.draw_texture(gallery_label, 20, 35)

            #  static branding
            app.draw_texture(logo, WIDTH - 84, 20, 64, 64)
            app.draw_texture(title_tex, (WIDTH - title_w) / 2, 20)
            hud_text = f"Fps={app.fps} | rot={math.degrees(rotation) % 360:5.1f}deg | hit_box: dist={best_hit.distance:.1f}, pt=({best_hit.point.y:.0f})"

            new_debug_tex = app.render_text(debug_font, hud_text, 255, 255, 0)
            if debug_tex is not None:
                app.destroy_texture(debug_tex)  # must free every frame's old texture, or this leaks fast
            debug_tex = new_debug_tex
            app.draw_texture(debug_tex, 10, HEIGHT - 100)

            app.end_frame()

        #  cleanup: every load_texture/load_font/render_text needs a match
        app.destroy_texture(logo)
        app.destroy_texture(title_tex)
        app.destroy_texture(gallery_label)
        if debug_tex is not None:
            app.destroy_texture(debug_tex)
        app.destroy_font(title_font)
        app.destroy_font(debug_font)


if __name__ == "__main__":
    main()
