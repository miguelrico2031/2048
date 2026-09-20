#pragma once

#include <raylib.h>
#include <string>

namespace core::app
{
	struct Data
	{
		std::string m_Name = "";
		int m_WindowWidth = 0;
		int m_WindowHeight = 0;
		Color m_ClearColor = BLACK;
	};

	struct DataSingletonComponent
	{
		Data m_Data;
	};
}