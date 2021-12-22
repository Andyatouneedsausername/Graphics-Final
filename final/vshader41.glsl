attribute  vec4 vPosition;
attribute  vec4 vColor;
varying vec4 color;

uniform mat4 model_view;
uniform mat4 camera_view;
uniform mat4 projection;
uniform bool shade;
uniform bool fog;
uniform bool dark;
uniform float fogfactor;

void main() 
{
  vec4 aColor;
  float fogFactor;
  gl_Position = projection*camera_view*model_view*vPosition;
  if (shade) {
    //    color =abs(vPosition.z)*vColor;
    aColor =(.5 + vPosition.y)*vColor;
    if (aColor.x > 0.9 ) {
      aColor.x = 0.9;
    }
    if (aColor.y > 0.9) {
      aColor.y = 0.9;
    }
    if (aColor.z > 0.9) {
      aColor.z = 0.9;
    }
  }else {
    aColor = vColor;
  }
  if (fog) {
    const float LOG2 = 1.442695;
    gl_FogFragCoord = length(camera_view*model_view*vPosition)/fogfactor;
    float fog_density=0.4;
    fogFactor = exp2( -fog_density * 
		      fog_density * 
		      gl_FogFragCoord * 
		      gl_FogFragCoord * 
		      LOG2 );
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    if (dark) {
      color = mix(vec4(0, 0, 0, 0),aColor, fogFactor);
    } else {
      color = mix(vec4(1, 1, 1, 0),aColor, fogFactor);
    }
  } else {
    color = aColor;
  }
}
