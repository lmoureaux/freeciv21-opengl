#include "sprite_provider.h"

#include <QRandomGenerator>

/**
 * @class sprite_provider
 * @brief Implements the sprite selection logic.
 */

/**
 * @struct sprite_provider::drawn_sprite
 * @brief Groups the information needed to draw a sprite on the screen.
 */

/**
 * @brief Constructor
 */
sprite_provider::sprite_provider(QObject *parent) : QObject(parent)
{
    // Load terrain sprites
    const auto terrains = {
        QStringLiteral("arctic"),
        QStringLiteral("desert"),
        QStringLiteral("forest"),
        QStringLiteral("grassland"),
        QStringLiteral("hills"),
        QStringLiteral("jungle"),
        QStringLiteral("mountains"),
        QStringLiteral("plains"),
        QStringLiteral("swamp"),
        QStringLiteral("tundra"),
    };
    for (const auto &terrain: terrains) {
        m_sprites.emplace_back(QStringLiteral("assets/")
                               + terrain
                               + QStringLiteral(".png"));
    }

    // Generate a random map
    for (int i = 0; i < m_width * m_height; ++i) {
        m_map.push_back(QRandomGenerator::global()->bounded(
                            int(m_sprites.size())));
    }
}

/**
 * @brief Returns the set of drawn_sprites to use to render the map.
 */
std::vector<sprite_provider::drawn_sprite> sprite_provider::render() const
{
    std::vector<drawn_sprite> sprites;
    for (int ix = 0; ix < m_width; ++ix) {
        for (int iy = 0; iy < m_height; ++iy) {
            int x = (iy % 2 == 1) ? (ix * 96 + 48) : (ix * 96);
            int y = iy * 24;
            int terrain = m_map[ix * m_height + iy];
            sprites.push_back(drawn_sprite{terrain, x, y});
        }
    }
    return sprites;
}
