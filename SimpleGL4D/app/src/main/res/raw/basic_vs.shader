attribute vec2 vPosition;
attribute vec3 vColor;
varying vec3 color;
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
void main() {
  gl_Position = projectionMatrix * modelViewMatrix * vec4(vPosition, 0.0, 1.0);
  color = vColor;
}
