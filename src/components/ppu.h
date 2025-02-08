#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include <map>

#include "ppuioregisters.h"
#include "../ROM/rom.h"
#include "../frame.h"


struct PatternTile {
    unsigned char pixels[8][8];
};

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
    PPU(PPUIORegisters& io_registers, IMemory& ppu_mem_map, RAM& oam);
    void set_vblank(bool enable);
    bool maybe_send_nmi();
    void load_chr_rom(const ROM& rom);
    void set_oam_addr(unsigned char value);
    Frame render_frame();

private:
    static const std::map<unsigned char, Color> color_palette;
    PPUIORegisters& m_io_registers;
    IMemory& m_ppu_mem_map;
    unsigned char vram_addr;
    RAM& m_oam;

    bool is_background_rendering_enable();
    bool is_sprite_rendering_enable();

    PatternTile get_pattern_tile(const uint16_t& pattern_table_addr, const int& tile_number);

    /**
     * @brief display_bg_tile_to_frame
     * @param pattern_tile Tile to draw containing indices
     * @param frame Frame
     * @param collision_mask Collision mask to fill, must be same size as frame
     * @param palette Palette id
     * @param x X position of top-left corner
     * @param y Y position of top-left corner
     */
    void display_bg_tile_to_frame(const PatternTile& pattern_tile, Frame& frame, CollisionMask& collision_mask,
                                  const unsigned char& palette, const unsigned char& x, const unsigned char& y);

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
     * @return true if collision with background happens when collision_check = true
     */
    bool display_sprite_tile_to_frame(const PatternTile& pattern_tile, Frame& frame, bool collision_check, const CollisionMask& bg_colision_mask,
                                      const unsigned char& palette, const unsigned char& x, const unsigned char& y, bool flip_h, bool flip_v);


    void draw_backdrop_color(Frame& frame);
    CollisionMask render_background(Frame& frame);
    TileInfo get_tile_info_from_nametables(const int& i, const int& j);

    /**
     * @brief render_sprites
     * @param frame
     * @param bg_collision_mask
     * @return true if collision sprite 0 happens.
     */
    bool render_sprites(Frame& frame, const CollisionMask& bg_collision_mask);

    uint16_t get_background_pattern_table_addr();
    uint16_t get_sprite_pattern_table_addr();
};

#endif // PPU_H
