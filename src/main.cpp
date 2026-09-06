#include "app.h"


int main()
{
	if (!App::GetInstance().Init())
		return 1;
	App::GetInstance().Run();
	return 0;
}
