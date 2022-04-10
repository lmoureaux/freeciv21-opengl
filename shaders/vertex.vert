#version 330

/*
 * Vertex shader
 * =============
 *
 * The vertex shader is run once for every triangle vertex. It sets its
 * position and passes arbitrary data to the fragment shader.
 */

// Variables received for each vertex
layout(location = 0) in vec2 xy; // Screen coordinates
layout(location = 1) in vec2 offset; // Per-sprite offset
layout(location = 2) in vec2 uv; // Coordinates within the texture
layout(location = 3) in float sprite_index; // Which sprite to use

// Uniforms are variables that are the same for every vertex in a draw call
uniform mat4 projection_matrix;

// Varying are data passed to the fragment shader (interpolating smoothly
// between vertices in a triangle).
// The name must match the one in the fragment shader.
out vec2 frag_uv; // Coordinates within the texture
flat out float frag_sprite_index; // Index in the texture array

void main(void)
{
    // The full position is the (sprite-wide) offset plus the relative
    // location of this vertex
    vec2 pos = offset + xy;

    // Use the projection matrix to set the position. The last coordinate
    // is a trick to allow translations by setting the appropriate elements
    // of the projection matrix.
    gl_Position = projection_matrix * vec4(pos.x, pos.y, 0.0, 1.0);

    // Pass uv directly to the fragment shader. The GPU will interpolate
    // automatically.
    frag_uv = uv;
    frag_sprite_index = sprite_index;
}
