#ifndef _H_APPLICATION_
#define _H_APPLICATION_

class Application
{
private:
	//	non-copyable.
	Application(const Application&);
	Application& operator=(const Application&) {};

public:
	inline Application() {}
	inline virtual ~Application() {}
	inline virtual void Init() {}
	inline virtual void Update(float deltaTime) {}
	inline virtual void Render(float aspectRatio) {}
	inline virtual void Shutdown() {}
};

#endif