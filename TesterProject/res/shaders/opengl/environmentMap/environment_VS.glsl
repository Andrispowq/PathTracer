#version 430

layout (location = 0) in vec3 position_VS;

layout (location = 0) out vec3 position_FS;

uniform mat4 m_transform;
uniform mat4 m_view;
uniform mat4 m_projection;

void main()
{
	mat4 rotView = mat4(mat3(m_view));
	vec4 clipPos = m_projection * rotView * vec4(position_VS, 1.0);
	gl_Position = clipPos.xyww;//m_projection * m_view * m_transform * vec4(position_VS, 1);
	
	position_FS = position_VS;
}