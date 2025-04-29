#pragma once

#ifdef RM_PLATFORM_WINDOWS

extern RayMagic::Application* RayMagic::CreateApplication();

int main(int argc, char** argv)
{
	RayMagic::Log::Init();
	RM_CORE_WARN("Initialized Log!");
	
	auto app = RayMagic::CreateApplication();
	app->Run();
	delete app;
}

#endif