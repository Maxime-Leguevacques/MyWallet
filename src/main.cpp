#include "app.h"


int main()
{
	App::GetInstance().Init();
	App::GetInstance().Run();
	return 0;
}
