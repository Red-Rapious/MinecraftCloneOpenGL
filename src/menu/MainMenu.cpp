#include "MainMenu.hpp"

MainMenu::MainMenu(GLFWwindow* window)
	: m_basicShader("res/shaders/Basic.shader"), m_logoImage("res/textures/minecraft_logo.png"), m_text("res/textures/ascii.png")
{
	m_vao.Bind();

	m_basicShader.Bind();
	m_basicShader.SetUniform1i("u_Texture", 0);

	m_basicShader.SetUniform4f("u_Color", 1.0, 1.0, 1.0, 1.0);
}

bool MainMenu::UpdateFrame(GLFWwindow* window)
{
	m_renderer.Clear();

	
	m_logoImage.RenderImage(window, m_vao, &m_basicShader, 800);
	m_text.PrintText(window, m_vao, &m_basicShader, "TEST", 0, 0, 10);

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		return false; // if enter is pressed, return false to ask the main loop to switch into game mode
	return true; // if not pressed, continue the main menu
}