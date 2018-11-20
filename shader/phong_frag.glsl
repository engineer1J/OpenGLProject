uniform sampler2D u_texid;
uniform sampler2D u_texid_score;

uniform mat4 u_view_matrix;

uniform vec3 u_light_vector;
uniform vec4 u_light_ambient;
uniform vec4 u_light_diffuse;
uniform vec4 u_light_specular;

uniform vec4 u_material_ambient;
uniform vec4 u_material_diffuse;
uniform vec4 u_material_specular;
uniform float u_material_shininess;

varying vec3 v_vertex_wc;
varying vec3 v_normal_wc;
varying vec2 v_texcoord;

vec4 directional_light(vec4 input_color)
{
  vec4 color = vec4(0);

  vec3 vertex_wc = v_vertex_wc;
  vec3 normal_wc = normalize(v_normal_wc);
  
  vec3 light_vector_wc = normalize(u_light_vector);
  vec3 light_incident_vector_wc = - light_vector_wc;
  vec3 reflect_vector_wc = reflect(light_incident_vector_wc, normal_wc);
  
  vec3 view_position_wc = vec3(u_view_matrix * vec4(0,0,0,1));
  vec3 view_vector_wc = view_position_wc - vertex_wc;
  view_vector_wc = normalize(view_vector_wc);

  color += (u_light_ambient * input_color);

  float ndotl = max(0.0, dot(normal_wc, light_vector_wc));
  color += (ndotl * u_light_diffuse * input_color);

  float rdotv = max(0.0, dot(reflect_vector_wc, view_vector_wc) );
  color += (pow(rdotv, u_material_shininess) * u_light_specular * u_material_specular);

  return color;
}

void main() 
{
  vec4 color1 = texture2D(u_texid, v_texcoord);
  vec4 color2 = texture2D(u_texid_score, v_texcoord);

  gl_FragColor = directional_light(color1*0.5 + color2*0.5);
gl_FragColor = directional_light(color1*0.5);
}
