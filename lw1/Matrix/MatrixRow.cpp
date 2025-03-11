#include <functional>
#include <ostream>

std::ostream& operator<<(std::ostream& os, const std::function<std::ostream&(std::ostream&)>& manip)
{
	return manip(os);
}
