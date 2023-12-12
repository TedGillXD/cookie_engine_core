#pragma once
#include "Array.hpp"
#include <cassert>
#include <string>
#include <functional>

namespace Cookie::Util {

	template<typename Type, bool bShouldDestruct = std::is_destructible<Type>::value>
	class Set {

	};

	template<typename Type, bool bShouldDestruct = std::is_destructible<Type>::value>
	class HashSet {
	public:	//constructors and destructor
		//constructors
		HashSet();
		HashSet(uint32_t bucketCount);
		HashSet(const HashSet& other);					//copy constructor
		HashSet& operator=(const HashSet& other);		//copy assignment operator
		HashSet(HashSet&& other) noexcept;				//move constructor
		HashSet& operator=(HashSet&& other) noexcept;	//move assignment operator
		~HashSet() = default;							//destructor

	private:	//internal functions
		inline uint32_t GetHashKey(const Type& data);
		inline void Reset();

	public:	//member functions
		void Insert(Type data);
		void Contain(Type data);
		void Erase(Type data);

		inline const uint32_t GetSize() { return _size; }
		inline const uint32_t GetBucketSize() { return _buckets.GetSize(); }

		// implement class Iterator, begin() and end() to support C++11 interation

	private:
		static const uint32_t defaultBucketSize = 13;

		Array<Array<Type>> _buckets;
		uint32_t _size;

#ifdef _DEBUG
	private:
		std::string _name;
#endif
	};

	template<typename Type, bool bShouldDestruct /*= false*/>
	HashSet<Type, bShouldDestruct>::HashSet() {
		_buckets.Resize(defaultBucketSize);
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	HashSet<Type, bShouldDestruct>::HashSet(uint32_t bucketCount) {
		_buckets.Resize(bucketCount);
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	HashSet<Type, bShouldDestruct>::HashSet(const HashSet& other) {
		_buckets.Clear();
		_buckets.Resize(other._buckets.GetSize());
		uint32_t newSize = 0;
		for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
			_buckets[i] = other._buckets[i];
			newSize += other._buckets[i].GetSize();
		}
		_size = newSize;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	HashSet<Type, bShouldDestruct>& HashSet<Type, bShouldDestruct>::operator=(const HashSet& other) {
		assert(this != std::addressof(other));
		if (this != std::addressof(other)) {
			_buckets.Clear();
			_buckets.Resize(other._buckets.GetSize());
			uint32_t newSize = 0;
			for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
				_buckets[i] = other._buckets[i];
				newSize += other._buckets[i].GetSize();
			}
			_size = newSize;
		}
		return *this;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	HashSet<Type, bShouldDestruct>::HashSet(HashSet&& other) noexcept {
		_buckets.Clear();
		_buckets.Resize(other._buckets.GetSize());
		uint32_t newSize = 0;
		for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
			_buckets[i] = other._buckets[i];
			newSize += other._buckets[i].GetSize();
		}
		_size = newSize;
		other.Reset();
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	HashSet<Type, bShouldDestruct>& HashSet<Type, bShouldDestruct>::operator=(HashSet&& other) noexcept {
		assert(this != std::addressof(other));
		if (this != std::addressof(other)) {
			_buckets.Clear();
			_buckets.Resize(other._buckets.GetSize());
			uint32_t newSize = 0;
			for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
				_buckets[i] = other._buckets[i];
				newSize += other._buckets[i].GetSize();
			}
			_size = newSize;
			other.Reset();
		}
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	uint32_t HashSet<Type, bShouldDestruct>::GetHashKey(const Type& data) {
		std::hash<Type> hasher;
		return hasher(data) % _buckets.GetSize();
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	void HashSet<Type, bShouldDestruct>::Reset() {
		_buckets.Clear();
		_size = 0;
	}


}