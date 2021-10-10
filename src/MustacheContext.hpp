#ifndef MUSTACHECONTEXT_HPP
#define MUSTACHECONTEXT_HPP

#include <map>
#include <mustache.hpp>
#include <string>
#include "MustacheTemplates.hpp"

namespace GenImageTool
{
    template <typename string_type>
    class MustacheContext : public kainjow::mustache::context<string_type>
    {
    public:
        MustacheContext(const kainjow::mustache::basic_data<string_type>* data)
            : kainjow::mustache::context<string_type>(data)
        {
            addPartial("indent", INDENT_TEMPLATE);

            addPartial("h_blockmap", H_BLOCKMAP_TEMPLATE);
            addPartial("h_collisionblockarray", H_COLLISIONBLOCKARRAY_TEMPLATE);
            addPartial("h_file", H_FILE_TEMPLATE);
            addPartial("h_palette", H_PALETTE_TEMPLATE);
            addPartial("h_sprite", H_SPRITE_TEMPLATE);
            addPartial("h_spritearray", H_SPRITEARRAY_TEMPLATE);
            addPartial("h_tilemap", H_TILEMAP_TEMPLATE);
            addPartial("h_tilemaparray", H_TILEMAPARRAY_TEMPLATE);
            addPartial("h_tileset", H_TILESET_TEMPLATE);

            addPartial("c_blockmap", C_BLOCKMAP_TEMPLATE);
            addPartial("c_collisionblockarray", C_COLLISIONBLOCKARRAY_TEMPLATE);
            addPartial("c_file", C_FILE_TEMPLATE);
            addPartial("c_palette", C_PALETTE_TEMPLATE);
            addPartial("c_spritearray", C_SPRITEARRAY_TEMPLATE);
            addPartial("c_tilemap", C_TILEMAP_TEMPLATE);
            addPartial("c_tilemaparray", C_TILEMAPARRAY_TEMPLATE);
            addPartial("c_tileset", C_TILESET_TEMPLATE);
        }

        virtual const kainjow::mustache::basic_data::basic_data<string_type>* get_partial
            (
            const string_type& name
            ) const override
        {
            const auto partial = m_partials.find(name);
            if (partial == m_partials.end())
            {
                throw std::runtime_error("Partial not found: " + name);
            }

            return &partial->second;
        }

    private:
        void addPartial
            (
            string_type name,
            string_type partial
            )
        {
            m_partials.insert(std::make_pair(name, kainjow::mustache::basic_data<string_type>(partial)));
        }

        mutable std::unordered_map<string_type, kainjow::mustache::basic_data<string_type>> m_partials;
    };
}

#endif
