#pragma once

namespace Cookie::Util {

	template<typename Type1, typename Type2>
	struct Pair {
		Type1 Item1;
		Type2 Item2;
	};
	
	template<typename Type1, typename Type2, bool bType1ShouldDestruct = std::is_destructible<Type1>::value, bool bType2ShouldDestruct = std::is_destructible<Type2>::value>
	class Map {

	};

	template<typename Type1, typename Type2, bool bType1ShouldDestruct = std::is_destructible<Type1>::value, bool bType2ShouldDestruct = std::is_destructible<Type2>::value>
	class HashMap {

	};

}