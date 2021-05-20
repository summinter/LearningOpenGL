#version 330 core
out vec4 FragColor;


// texture samplers
uniform vec3 ourColor;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(ourColor,0.5);
}