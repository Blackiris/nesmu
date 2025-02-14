#ifndef PPU_H
#define PPU_H

#include "ram.h"
#include <map>
#include <vector>

#include "ppu_io_registers.h"
#include "ppu_memory_map.h"
#include "../ROM/rom.h"
#include "../frame.h"
#include "pattern_tile.h"


struct CollisionMask {
    std::vector<bool> pixels;
};

struct TileInfo {
    uint8_t tile_id;
    uint8_t palette_byte;
};


class PPU
{
public:
    PPU(PPUIORegisters& io_registers, PPUMemoryMap& ppu_mem_map, RAM& oam);
    void set_vblank(bool enable);
    bool maybe_send_nmi();
    void load_chr_rom(const ROM& rom);
    void set_oam_addr(const uint8_t& value);
    void render_frame_scanline(Frame& frame, const int& line_number);
    void draw_backdrop_color(Frame& frame);

private:
    static const std::map<uint8_t, Color> color_palette;
    PPUIORegisters& m_io_registers;
    PPUMemoryMap& m_ppu_mem_map;
    uint8_t vram_addr;
    RAM& m_oam;

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
     * @param scanline scanline to display
     */
    void display_bg_tile_to_frame_line(const PatternTile& pattern_tile, Frame& frame, CollisionMask& collision_mask,
                                       const uint8_t& palette, const int& x, const int& y, const int& scanline);
    /**
     * @brief display_sprite_tile_to_frame
     * @param pattern_tile Tile to draw containing indices
     * @param frame Frame
     * @param bg_colision_mask Collision mask to check sprite 0, must be same size as frame
     * @param palette Palette id
     * @param x X position of top-left corner
     * @param y Y position of top-left corner
     * @param priority false in front of background, true behind
     * @param flip_h Enable horizontal flip
     * @param flip_v Enable vertical flip
     * @param scanline scanline to display
     * @return true if collision with background happens when collision_check = true
     */
    bool display_sprite_tile_to_frame_line(const PatternTile& pattern_tile, Frame& frame, const CollisionMask& bg_collision_mask,
                                      const uint8_t& palette, const uint8_t& x, const uint8_t& y,
                                      const bool& priority, const bool& flip_h, const bool& flip_v, const int& scanline);


    CollisionMask render_background_line(Frame& frame, const int& scanline);
    TileInfo get_tile_info_from_nametables(const int& i, const int& j);

    /**
     * @brief render_sprites
     * @param frame
     * @param bg_collision_mask
     * @param scanline scanline to display
     * @return true if collision sprite 0 happens.
     */
    bool render_sprites_line(Frame& frame, const CollisionMask& bg_collision_mask, const int& scanline);

    uint16_t get_background_pattern_table_addr();
    uint16_t get_sprite_pattern_table_addr();
};

#endif // PPU_H
