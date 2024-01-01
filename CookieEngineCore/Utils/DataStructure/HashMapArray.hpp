#pragma once
#include <vector>
#include <unordered_map>
#include "Common/MacroDefinition.h"
#include <shared_mutex>

namespace Cookie::Util {

	template<typename ValueType, typename KeyType = std::string>
	class HashMapArray {
		static_assert(std::is_default_constructible<ValueType>::value && std::is_default_constructible<KeyType>::value, 
			"Key type and value type should default constructible");

	public:
		HashMapArray() = default;
		HashMapArray(uint32_t threshold);
		DISABLE_MOVE_AND_COPY(HashMapArray);

	public:
		//insert value into it and return its index
		uint32_t Insert(KeyType&& key, ValueType&& value);
		uint32_t Insert(const KeyType& key, const ValueType& value);
		ValueType& At(KeyType key);
		// access element by index doesn't guarentee that the element has been removed
		ValueType& At(uint32_t index);
		void Remove(uint32_t index);
		void Remove(KeyType key);
		bool IsContain(KeyType key);
		bool IsContain(uint32_t index);
		uint32_t GetIndexByKey(const KeyType& key);
		uint32_t GetIndexByKey(KeyType&& key);

	private:
		std::shared_mutex _mutex;

		std::vector<ValueType> _arr;
		std::unordered_map<KeyType, uint32_t> _keyToIndex;
		std::unordered_map<uint32_t, KeyType> _indexToKey;
		std::vector<uint32_t> _availableIndex;

		uint32_t _arrThreshold = 1023;	// when the size of _arr exceed this threshold, it will start to use available index
	};

	template<typename ValueType, typename KeyType /*= std::string*/>
	uint32_t HashMapArray<ValueType, KeyType>::GetIndexByKey(KeyType&& key) {
		std::shared_lock<std::shared_mutex> lock(_mutex);
		if (_keyToIndex.count(key)) return 0;
		return _keyToIndex[key];
	}

	template<typename ValueType, typename KeyType /*= std::string*/>
	uint32_t HashMapArray<ValueType, KeyType>::GetIndexByKey(const KeyType& key) {
		std::shared_lock<std::shared_mutex> lock(_mutex);
		if(_keyToIndex.count(key)) return 0;
		return _keyToIndex[key];
	}

	// read
	template<typename ValueType, typename KeyType /*= std::string*/>
	bool HashMapArray<ValueType, KeyType>::IsContain(KeyType key) {
		std::shared_lock<std::shared_mutex> lock(_mutex);
		return _keyToIndex.count(key);
	}

	template<typename ValueType, typename KeyType /*= std::string*/>
	bool HashMapArray<ValueType, KeyType>::IsContain(uint32_t index) {
		std::shared_lock<std::shared_mutex> lock(_mutex);
		return _indexToKey.count(index);
	}

	template<typename ValueType, typename KeyType /*= std::string*/>
	HashMapArray<ValueType, KeyType>::HashMapArray(uint32_t threshold) {
		this->_arrThreshold = threshold;
	}

	//write
	template<typename ValueType, typename KeyType /*= std::string*/>
	uint32_t HashMapArray<ValueType, KeyType>::Insert(KeyType&& key, ValueType&& value) {
		std::unique_lock<std::shared_mutex> lock(_mutex);
		uint32_t index = 0;
		if (!_availableIndex.empty() && _arr.size() > _arrThreshold) {
			index = _availableIndex.back();
			_availableIndex.pop_back();
			// get old key
			KeyType oldKey;
			if (_indexToKey.count(index)) {
				oldKey = _indexToKey[index];
			}
			_indexToKey[index] = key;

			// eliminate old value from hash map
			_keyToIndex.erase(oldKey);
			_keyToIndex[key] = index;

			// add new value
			_arr[index] = value;

		} else {
			index = (uint32_t)_arr.size();
			_arr.push_back(value);
			_keyToIndex[key] = index;
			_indexToKey[index] = key;
		}

		return index;
	}

	template<typename ValueType, typename KeyType /*= std::string*/>
	uint32_t HashMapArray<ValueType, KeyType>::Insert(const KeyType& key, const ValueType& value) {
		std::unique_lock<std::shared_mutex> lock(_mutex);
		uint32_t index = 0;
		if (!_availableIndex.empty() && _arr.size() > _arrThreshold) {
			index = _availableIndex.back();
			_availableIndex.pop_back();
			// get old key
			KeyType oldKey;
			if (_indexToKey.count(index)) {
				oldKey = _indexToKey[index];
			}
			_indexToKey[index] = key;

			// eliminate old value from hash map
			_keyToIndex.erase(oldKey);
			_keyToIndex[key] = index;

			// add new value
			_arr[index] = value;

		}
		else {
			index = (uint32_t)_arr.size();
			_arr.push_back(value);
			_keyToIndex[key] = index;
			_indexToKey[index] = key;
		}

		return index;
	}

	// read
	template<typename ValueType, typename KeyType /*= std::string*/>
	ValueType& HashMapArray<ValueType, KeyType>::At(KeyType key) {
		std::shared_lock<std::shared_mutex> lock(_mutex);
		assert(_keyToIndex.count(key));
		if (!_keyToIndex.count(key)) {
			throw std::out_of_range("key not found!");
		}

		assert(_keyToIndex[key] < _arr.size());
		return _arr[_keyToIndex[key]];
	}

	// read
	template<typename ValueType, typename KeyType /*= std::string*/>
	ValueType& HashMapArray<ValueType, KeyType>::At(uint32_t index) {
		std::shared_lock<std::shared_mutex> lock(_mutex);
		assert(index < _arr.size());
		if (index >= _arr.size()) {
			throw std::out_of_range("key not found!");
		}
		return _arr[index];
	}

	// write
	template<typename ValueType, typename KeyType /*= std::string*/>
	void HashMapArray<ValueType, KeyType>::Remove(uint32_t index) {
		std::unique_lock<std::shared_mutex> lock(_mutex);
		assert(index < _arr.size());
		_availableIndex.push_back(index);
		KeyType& oldKey = _indexToKey[index];
		_indexToKey.erase(index);
		_keyToIndex.erase(oldKey);
	}

	// write
	template<typename ValueType, typename KeyType /*= std::string*/>
	void HashMapArray<ValueType, KeyType>::Remove(KeyType key) {
		std::unique_lock<std::shared_mutex> lock(_mutex);
		assert(_keyToIndex.count(key));
		uint32_t index = _keyToIndex[key];
		_availableIndex.push_back(index);
		_keyToIndex.erase(key);
		_indexToKey.erase(index);
	}


}