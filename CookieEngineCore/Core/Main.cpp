#include "Utils/Utils.h"
#include <iostream>
#include <unordered_set>

int main() {
	Cookie::Util::HashSet<int> s;
	Cookie::Util::Array<int> arr;
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);
	arr.PushBack(100);

 	for (auto& num : arr) {
 		std::cout << num << " ";
 	}
 	std::cout << "\n";

	return 0;
}