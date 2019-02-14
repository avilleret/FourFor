precision highp float; // this will make the default precision high

//we passed this in from our vert shader
varying vec2        texcoord0;

//These are variables we set in our ofApp using the ofShader API

//our texture reference
//passed in by
//shader.setUniformTexture("tex0", sourceImage.getTextureReference(), sourceImage.getTextureReference().texData.textureID);
uniform sampler2D   tex0;
uniform sampler2D   tex1;
// normalized texture coord (0..1)

//width and height that we are working with
//passed in by
//shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
uniform vec2        resolution;

//a changing value to work with
//passed in by
//shader.setUniform1f("time", ofGetElapsedTimef());
uniform float       time;

uniform float freq;
uniform float amplitude;
uniform float alpha; // temporal feedback amount
uniform float beta; // scaling factor
uniform vec2 anchor; // scaling anchor


//Each shader has one main() function you can use
//Below are a few implementations. Make sure you have all but one commented out

//Shaders are compiled at runtime meaning that you can just change the shader file
//and re-run the ofApp without compiling


// just draw the texture to screen
/*void main()
{
    gl_FragColor = texture2D(tex0, texcoord0);
}*/



// draw the texture to screen, inverted
/*void main()
{
    vec4 texColor = texture2D(tex0, texcoord0);
    gl_FragColor = vec4(1.0-texColor.r, 1.0-texColor.g, 1.0-texColor.b, texColor.a);
}*/




// let's wobble the image channels around independently, a bit Fear and Loathing in Las Vegas style
void main()
{
    mediump float newTime = time * freq;

    vec2 newTexCoord = texcoord0;
    newTexCoord.s = texcoord0.s + (cos(newTime + (texcoord0.s*20.0)) * 0.01);
    newTexCoord.t = texcoord0.t + (sin(newTime + (texcoord0.t*20.0)) * 0.01);

    mediump vec2 texCoordRed    = newTexCoord;
    mediump vec2 texCoordGreen  = newTexCoord;
    mediump vec2 texCoordBlue   = newTexCoord;

    texCoordRed     += vec2( cos((newTime * 2.76)), sin((newTime * 2.12)) )* 0.01 * amplitude;
    texCoordGreen   += vec2( cos((newTime * 2.23)), sin((newTime * 2.40)) )* 0.01 * amplitude;
    texCoordBlue    += vec2( cos((newTime * 2.98)), sin((newTime * 2.82)) )* 0.01 * amplitude;

    mediump float colorR = texture2D( tex0, texCoordRed ).r;
    mediump float colorG = texture2D( tex0, texCoordGreen).g;
    mediump float colorB = texture2D( tex0, texCoordBlue).b;
    mediump float colorA = texture2D( tex0, texCoordBlue).a;
    mediump vec4 outColor = vec4( colorR, colorG, colorB, colorA);

    mediump vec2 ghost_coord = texcoord0 + (beta-1.)*(anchor - texcoord0);

    outColor = ((1.-alpha) * outColor + alpha*texture2D(tex1, ghost_coord));

    gl_FragColor = outColor;
}


/*
//This is the internal RPi vert shader for reference
precision lowp float;

uniform sampler2D src_tex_unit0;

uniform float usingTexture;
uniform float bitmapText;

varying vec4 colorVarying;
varying vec2 texCoordVarying;

void main(){
  vec4 tex;
  if(usingTexture>.5){
    tex = texture2D(src_tex_unit0, texCoordVarying);
    if(bitmapText>.5 && tex.a < 0.5){
      discard;
    }else{
      gl_FragColor = colorVarying*tex;
    }
  }else{
    gl_FragColor = colorVarying;
  }
}
*/
