in vec3 vertex_position;

out vec4 pos;

void main() {
  pos = vec4(vertex_position, 1.0);
}
