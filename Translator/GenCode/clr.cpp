#include <string>
#include <iostream>

namespace pr
{
int i;
void pr()
{
i = 100;
{
if (i==100)std::cout << (1);
else std::cout << (0);
}
while (i>95){
std::cout << (i);
i = i-1;
}
do
i = 10;
while (i!=10);
for (i = 5; i >= 0; i--)
std::cout << (i);
}
}

int main()
{
pr::pr();
System::Console::ReadKey();
}