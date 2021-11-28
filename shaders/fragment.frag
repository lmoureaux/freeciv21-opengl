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
varying vec2 frag_uv; // Coordinates within the texture

// Uniforms are variables that are the same for every vertex in a draw call
uniform sampler2D sprite; // Texture "samplers" retrieve texture data

void main(void)
{
    // Trivial :)
    gl_FragColor = texture2D(sprite, frag_uv);
}
