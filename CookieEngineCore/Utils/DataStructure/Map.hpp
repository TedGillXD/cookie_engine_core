#pragma once
#include "Array.hpp"
#include <string>
#include <functional>
#include <stdexcept>
#include <initializer_list>

#pragma warning(disable : 4715)

namespace Cookie::Util {

	template<typename Type1, typename Type2>
	struct Pair {
		static_assert(std::is_default_constructible<Type1>::value&& std::is_default_constructible<Type2>::value, "Type1 and Type2 should be defualt constructible!");

		Type1 Item1;
		Type2 Item2;

	public:
		Pair();
		Pair(Type1 item1, Type2 item2);
	};

	template<typename Type1, typename Type2>
	Pair<Type1, Type2>::Pair() {
		Item1 = Type1{};
		Item2 = Type2{};
	}

	template<typename Type1, typename Type2>
	Pair<Type1, Type2>::Pair(Type1 item1, Type2 item2) {
		Item1 = item1;
		Item2 = item2;
	}


	template<typename Type1, typename Type2>
	class Map {

	};

	template<typename Type1, typename Type2>
	class HashMap {
		static_assert(std::is_default_constructible<Type1>::value && std::is_default_constructible<Type2>::value, "Type1 and Type2 should be default constructible!");

		DEBUG_INFO()

	public:
		HashMap();
		HashMap(uint32_t bucketSize);
		HashMap(Array<Pair<Type1, Type2>>& arr);
		HashMap(std::initializer_list<Pair<Type1, Type2>> initList);
		HashMap(const HashMap& other);									//copy constructor
		HashMap& operator=(const HashMap& other);						//copy assignment operator
		HashMap(HashMap&& other);										//move constructor
		HashMap& operator=(HashMap&& other);							//move assignment operator
		~HashMap() = default;											//destructor

	private:	// internal member functions
		inline uint32_t GetHashKey(const Type1& key);
		inline void Reset();
		inline void Init();

	public:		// member functions
		void Insert(Pair<Type1, Type2> data);
		void Erase(const Type1& key);
		[[nodiscard]] bool ContainKey(const Type1& key);
		[[nodiscard]] Type2& GetValue(const Type1& key);
		[[nodiscard]] const Type2 GetValue(const Type1& key) const;
		[[nodiscard]] Type2& operator[](const Type1& key);
		[[nodiscard]] const Type2& operator[](const Type1& key) const;

		inline const uint32_t GetSize() const { return _size; }
		inline const uint32_t GetBucketSize() const { return _buckets.GetSize(); }

		//TODO: implement class Interator, begin() and end() to support C++11 interation
		class Iterator {
		public:
			Iterator(Array<Array<Pair<Type1, Type2>>>* ptr) : _ptr(ptr), currentBucketIndex(-1), currentIndex(0) {
				currentBucketIndex = GetNextAvailable(currentBucketIndex);
			}
			Iterator(Array<Array<Pair<Type1, Type2>>>* ptr, uint32_t bucketIndex) : _ptr(ptr), currentBucketIndex(bucketIndex), currentIndex(0) { }

			inline Pair<Type1, Type2>& operator*() const { return (*_ptr)[currentBucketIndex][currentIndex]; }
			inline Iterator& operator++() {
				currentIndex++;
				if (currentIndex >= (*_ptr)[currentBucketIndex].GetSize()) {
					currentIndex = 0;
					currentBucketIndex = GetNextAvailable(currentBucketIndex);
				}
				return *this;
			}
			inline bool operator!=(const Iterator& other) const {
				return (_ptr != other._ptr || currentBucketIndex != other.currentBucketIndex || currentIndex != other.currentIndex);
			}
			inline bool operator==(const Iterator& other) const {
				return (_ptr == other._ptr && currentBucketIndex == other.currentBucketIndex && currentIndex == other.currentIndex);
			}

		private:
			[[nodiscard]] inline uint32_t GetNextAvailable(uint32_t current) {
				current++;
				while (current < (*_ptr).GetSize()) {
					if ((*_ptr)[current].GetSize() == 0) {
						current++;
					}
					else {
						break;
					}
				}
				return current;
			}

		private:
			Array<Array<Pair<Type1, Type2>>>* _ptr;
			uint32_t currentBucketIndex;
			uint32_t currentIndex;
		};

		[[nodiscard]] inline Iterator begin() { return Iterator(&_buckets); }
		[[nodiscard]] inline Iterator end() { return Iterator(&_buckets, _buckets.GetSize()); }
		[[nodiscard]] Iterator begin() const { return Iterator(&_buckets); }
		[[nodiscard]] inline Iterator end() const { return Iterator(&_buckets, _buckets.GetSize()); }

	private:
		static const uint32_t defaultBucketSize = 13;

		Array<Array<Pair<Type1, Type2>>> _buckets;
		uint32_t _size = 0;
	};

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>::HashMap() {
		_buckets.Resize(defaultBucketSize);
		Init();
		OBJECT_CREATE("HashMap");
	}

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>::HashMap(uint32_t bucketSize) {
		_buckets.Resize(bucketSize);
		Init();
		OBJECT_CREATE("HashMap");
	}

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>::HashMap(Array<Pair<Type1, Type2>>& arr) {
		_buckets.Resize(defaultBucketSize);
		Init();
		for (auto& item : arr) {
			Insert(item);
		}
		OBJECT_CREATE("HashMap");
	}

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>::HashMap(std::initializer_list<Pair<Type1, Type2>> initList) {
		_buckets.Resize(defaultBucketSize);
		Init();
		for (auto& item : initList) {
			Insert(item);
		}
		OBJECT_CREATE("HashMap");
	}

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>::HashMap(const HashMap& other) {
		_buckets.Clear();
		_buckets.Resize(other.GetBucketSize());
		Init();
		uint32_t newSize = 0;
		for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
			this->_buckets[i] = other._buckets[i];
			newSize += other._buckets[i].GetSize();
		}
		_size = newSize;
		OBJECT_CREATE("HashMap");
	}

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>& HashMap<Type1, Type2>::operator=(const HashMap& other) {
		assert(this != &other);
		if (this != &other) {
			_buckets.Clear();
			_buckets.Resize(other.GetBucketSize());
			Init();
			uint32_t newSize = 0;
			for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
				this->_buckets[i] = other._buckets[i];
				newSize += other._buckets[i].GetSize();
			}
			_size = newSize;
		}
		OBJECT_CREATE("HashMap");
		return *this;
	}

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>::HashMap(HashMap&& other) {
		_buckets.Clear();
		_buckets.Resize(other.GetBucketSize());
		Init();
		uint32_t newSize = 0;
		for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
			this->_buckets[i] = other._buckets[i];
			newSize += other._buckets[i].GetSize();
		}
		other.Reset();
		_size = newSize;
		OBJECT_CREATE("HashMap");
	}

	template<typename Type1, typename Type2>
	HashMap<Type1, Type2>& HashMap<Type1, Type2>::operator=(HashMap&& other) {
		assert(this != &other);
		if (this != &other) {
			_buckets.Clear();
			_buckets.Resize(other.GetBucketSize());
			Init();
			uint32_t newSize = 0;
			for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
				this->_buckets[i] = other._buckets[i];
				newSize += other._buckets[i].GetSize();
			}
			other.Reset();
			_size = newSize;
		}
		OBJECT_CREATE("HashMap");
		return *this;
	}

	template<typename Type1, typename Type2>
	uint32_t HashMap<Type1, Type2>::GetHashKey(const Type1& key) {
		std::hash<Type1> hasher;
		return hasher(key) % _buckets.GetSize();
	}

	template<typename Type1, typename Type2>
	void HashMap<Type1, Type2>::Reset() {
		_buckets.Clear();
		_size = 0;
	}

	template<typename Type1, typename Type2>
	void HashMap<Type1, Type2>::Init() {
		uint32_t size = _buckets.GetSize();
		for (uint32_t i = 0; i < size; i++) {
			_buckets[i] = Array<Pair<Type1, Type2>>();
		}
	}

	template<typename Type1, typename Type2>
	void HashMap<Type1, Type2>::Insert(Pair<Type1, Type2> data) {
		assert(!ContainKey(data.Item1));
		if(ContainKey(data.Item1)) return;
		_buckets[GetHashKey(data.Item1)].PushBack(data);
		_size++;
	}

	template<typename Type1, typename Type2>
	void HashMap<Type1, Type2>::Erase(const Type1& key) {
		assert(ContainKey(key));
		if(!ContainKey(key)) return;
		Array<Pair<Type1, Type2>>& tempArr = _buckets[GetHashKey(key)];
		for (uint32_t i = 0; i < tempArr.GetSize(); i++) {
			if (tempArr[i].Item1 == key) {
				tempArr.Erase(i);
				break;
			}
		}
		_size--;
	}

	template<typename Type1, typename Type2>
	bool HashMap<Type1, Type2>::ContainKey(const Type1& key) {
		for (Pair<Type1, Type2>& item : _buckets[GetHashKey(key)]) {
			if (item.Item1 == key) {
				return true;
			}
		}
		return false;
	}

	template<typename Type1, typename Type2>
	Type2& HashMap<Type1, Type2>::GetValue(const Type1& key) {
		assert(ContainKey(key));
		for (Pair<Type1, Type2>& item : _buckets[GetHashKey(key)]) {
			if (item.Item1 == key) {
				return item.Item2;
			}
		}
	}

	template<typename Type1, typename Type2>
	const Type2 HashMap<Type1, Type2>::GetValue(const Type1& key) const {
		assert(ContainKey(key));
		for (Pair<Type1, Type2>& item : _buckets[GetHashKey(key)]) {
			if (item.Item1 == key) {
				return item.Item2;
			}
		}
		return Type2{};
	}

	template<typename Type1, typename Type2>
	Type2& HashMap<Type1, Type2>::operator[](const Type1& key) {
		Array<Pair<Type1, Type2>>& tempArr = _buckets[GetHashKey(key)];
		if (ContainKey(key)) {
			for (Pair<Type1, Type2>& item : tempArr) {
				if (item.Item1 == key) {
					return item.Item2;
				}
			}
		} else {
			tempArr.PushBack({ key, Type2{} });
			return tempArr.Back().Item2;
		}
	}

	template<typename Type1, typename Type2>
	const Type2& HashMap<Type1, Type2>::operator[](const Type1& key) const {
		Array<Pair<Type1, Type2>>& tempArr = _buckets[GetHashKey(key)];
		if (ContainKey(key)) {
			for (Pair<Type1, Type2>& item : tempArr) {
				if (item.Item1 == key) {
					return item.Item2;
				}
			}
		}
		else {
			throw std::out_of_range("Key not found!");
		}
		return Type2{};
	}
}

#pragma warning(default : 4715)