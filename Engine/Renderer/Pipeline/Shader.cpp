#include "Shader.hpp"
#include "Engine/Renderer/Pipeline/ShaderProgram.hpp"
#include "Engine/Renderer/Pipeline/RenderState.hpp"

//===============================================================================================
Shader::Shader(const String& name, ShaderProgram* program, RenderState* state)
{
	m_name = name;
	m_program = program;
	m_state = state;
}

//-----------------------------------------------------------------------------------------------
Shader::~Shader()
{
	delete m_program;
	m_program = nullptr;

	delete m_state;
	m_state = nullptr;
}