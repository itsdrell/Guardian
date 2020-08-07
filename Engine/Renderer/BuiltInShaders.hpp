#pragma once

//====================================================================================
// Forward Declare
//====================================================================================
class Shader;

//====================================================================================
// Type Defs + Defines
//====================================================================================


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class BuiltInShaders
{
public:

	static void CreateAllBuiltInShaders();

	static Shader* CreateDefaultShader();
	static Shader* CreateInvalidShader();
	//static Shader* CreatePassThroughShader();


	// This is for the shader program to use if it has problems create a shader it can use the
	// invalid fs and vs source to create a new program
	static const char* GetInvalidFragment();
	static const char* GetInvalidVertex();


public:
};

//====================================================================================
// Standalone C Functions
//====================================================================================


//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [1/31/2019]
//====================================================================================
