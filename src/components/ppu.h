#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include <map>

#include "ppuioregisters.h"
#include "ppumemorymap.h"
#include "../ROM/rom.h"
#include "../frame.h"
#include "pattern_tile.h"


struct CollisionMask {
    bool pixels[256][240];
};

struct TileInfo {
    unsigned char tile_id;
    unsigned char palette_byte;
};


class PPU
{
public:
    PPU(PPUIORegisters& io_registers, PPUMemoryMap& ppu_mem_map, RAM& oam);
    void set_vblank(bool enable);
    bool maybe_send_nmi();
    void load_chr_rom(const ROM& rom);
    void set_oam_addr(unsigned char value);
    void render_frame_scanline(Frame& frame, const int& line_number);
    void draw_backdrop_color(Frame& frame);

private:
    static const std::map<unsigned char, Color> color_palette;
    PPUIORegisters& m_io_registers;
    PPUMemoryMap& m_ppu_mem_map;
    unsigned char vram_addr;
    RAM& m_oam;
    bool m_check_sprite_0_collision = true;

    bool is_background_rendering_enable();
    bool is_sprite_rendering_enable();

    /**
     * @brief display_bg_tile_to_frame
     * @param pattern_tile Tile to draw containing indices
     * @param frame Frame
     * @param collision_mask Collision mask to fill, must be same size as frame
     * @param palette Palette id
     * @param x X position of top-left corner
     * @param y Y position of top-left corner
     * @param line_number scanline to display
     */
    void display_bg_tile_to_frame_line(const PatternTile& pattern_tile, Frame& frame, CollisionMask& collision_mask,
                                       const unsigned char& palette, const unsigned char& x, const unsigned char& y, const int& line_number);
    /**
     * @brief display_sprite_tile_to_frame
     * @param pattern_tile Tile to draw containing indices
     * @param frame Frame
     * @param bg_colision_mask Collision mask to check sprite 0, must be same size as frame
     * @param collision_check true to enable collision check
     * @param palette Palette id
     * @param x X position of top-left corner
     * @param y Y position of top-left corner
     * @param flip_h Enable horizontal flip
     * @param flip_v Enable vertical flip
     * @param line_number scanline to display
     * @return true if collision with background happens when collision_check = true
     */
    bool display_sprite_tile_to_frame_line(const PatternTile& pattern_tile, Frame& frame, bool collision_check, const CollisionMask& bg_collision_mask,
                                      const unsigned char& palette, const unsigned char& x, const unsigned char& y, bool flip_h, bool flip_v, const int& line_number);


    CollisionMask render_background_line(Frame& frame, const int& line_number);
    TileInfo get_tile_info_from_nametables(const int& i, const int& j);

    /**
     * @brief render_sprites
     * @param frame
     * @param bg_collision_mask
     * @param line_number scanline to display
     * @return true if collision sprite 0 happens.
     */
    bool render_sprites_line(Frame& frame, const CollisionMask& bg_collision_mask, const int& line_number);

    uint16_t get_background_pattern_table_addr();
    uint16_t get_sprite_pattern_table_addr();
};

#endif // PPU_H
