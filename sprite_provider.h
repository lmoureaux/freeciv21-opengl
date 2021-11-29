#ifndef SPRITE_PROVIDER_H
#define SPRITE_PROVIDER_H

#include <QIcon>
#include <QObject>

#include <vector>

class sprite_provider : public QObject
{
    Q_OBJECT
public:
    struct drawn_sprite
    {
        int sprite_index;
        int x, y;
    };

    explicit sprite_provider(QObject *parent = nullptr);

    std::vector<QIcon> sprites() const { return m_sprites; }
    std::vector<drawn_sprite> render() const;

private:
    const int m_width = 20, m_height = 40;
    std::vector<QIcon> m_sprites;
    std::vector<int> m_map;
};

#endif // SPRITE_PROVIDER_H
