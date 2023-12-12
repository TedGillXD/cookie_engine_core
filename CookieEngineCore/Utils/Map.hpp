#pragma once

namespace Cookie::Util {

	template<typename Type1, typename Type2>
	struct Pair {
		Type1 Item1;
		Type2 Item2;
	};
	
	template<typename Type1, typename Type2, bool bType1ShouldDestruct = false, bool bType2ShouldDestruct = false>
	class Map {

	};

	template<typename Type1, typename Type2, bool bType1ShouldDestruct = false, bool bType2ShouldDestruct = false>
	class HashMap {

	};

}