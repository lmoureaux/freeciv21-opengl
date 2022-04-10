#version 330

/*
 * Fragment shader
 * ===============
 *
 * The fragment shader is run once for every pixel on the screen (unless
 * when MSAA is in use). It sets the final pixel color. It is also possible
 * to use it to save arbitrary data, which is e.g. used for shadows.
 */

// Varying are data passed to the fragment shader (interpolating smoothly
// between vertices in a triangle).
// The name must match the one in the fragment shader.
in vec2 frag_uv; // Coordinates within the texture
flat in float frag_sprite_index; // Index within the array

// Uniforms are variables that are the same for every vertex in a draw call
uniform sampler2DArray atlas; // Texture "samplers" retrieve texture data

void main(void)
{
    // Trivial :)
    gl_FragColor = texture(atlas, vec3(frag_uv, frag_sprite_index));

    // Manipulating the color here is basically the only way to debug things
    // For instance, this is the basic thing to do when you can't see
    // anything (draw flat red):
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    // This shows your UV coordinates
//    gl_FragColor.gb = frag_uv;
/*
    if (abs(frag_uv.x - 1.0) < 0.01 || abs(frag_uv.x) < 0.01) {
        gl_FragColor.r = 1.0;
    }
    if (abs(frag_uv.y - 1.0) < 0.01 || abs(frag_uv.y) < 0.01) {
        gl_FragColor.g = 1.0;
    }
    if (gl_FragColor.a < 0.1) {
        gl_FragColor.b = 1.0;
    }
*/
}
