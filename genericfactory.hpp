#ifndef GENERIC_FACTORY_HPP
#define GENERIC_FACTORY_HPP

#include <functional>
#include <memory>
#include <map>
#include <stdexcept>

/*
 * MIT License
 *
 * Copyright (c) 2025 Yanujz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * provided to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

template<typename BaseClass, typename Key = unsigned long>
class GenericFactory
{
public:
    typedef std::function<BaseClass* ()>   creatorFunc;
    typedef std::map<Key, creatorFunc>     creators;

private:
    struct initializer
    {
        const Key         key;
        const creatorFunc fn;
    };


public:
    GenericFactory() = default;

    /**
     * @brief Construct a new Generic Factory object
     *
     * @param l
     */
    GenericFactory(const std::initializer_list<initializer> &l)
    {
        for(auto const &el : l)
        {
            if(_creators.find(el.key) != _creators.end())
            {
                throw std::runtime_error("Duplicate key in initializer list");
            }
            add(el.key, el.fn);
        }
    }

    virtual ~GenericFactory() = default;

    /**
     * @brief Add a creator function for a specific key
     *
     * @param key
     * @param creator
     */
    void add(const Key &key, const creatorFunc creator)
    {
        _creators[key] = creator;
    }

    /**
     * @brief Remove a creator function for a specific key
     *
     * @param[in] key
     * @return true if the key was found and removed
     * @return false if the key was not found
     */
    bool remove(const Key &key)
    {
        auto it = _creators.find(key);

        if(it == _creators.end())
            return false;

        return _creators.erase(key) > 0;
    }

    /**
     * @brief Get an object created by a registered creator function
     *
     * @param str The key to search for
     * @return std::shared_ptr<BaseClass> A shared pointer to the created object
     */
    std::shared_ptr<BaseClass> get(const Key &str)
    {
        auto it = _available.find(str);

        if(it == _available.end())
        {
            auto it1 = _creators.find(str);
            if(it1 == _creators.end())
                return nullptr;

            _available.emplace(str, std::shared_ptr<BaseClass>(it1->second()));
        }
        return _available[str];
    }

    /**
     * @brief Get all registered creators
     *
     * @return const creators& A reference to the map of registered creators
     */
    const creators& registered() const
    {
        return _creators;
    }

    /**
     * @brief Stop the availability of a created object
     *
     * @param key The key of the object to stop
     * @return true if the object was successfully stopped
     * @return false if the object was not found
     */
	bool stop(const Key& key)
	{
		auto it = _available.find(key);

		if(it == _available.end())
			return false;

		return _available.erase(key) > 0;
	}

    /**
     * @brief A static creator function template for creating instances of a specific type
     * 
     * @tparam T The type to create, which must derive from BaseClass
     * @return A pointer to the created object
     */
    template<typename T>
    static T* creator()
    {
        static_assert(std::is_base_of<BaseClass, T>::value, "T must derive from BaseClass");
        return new T;
    }
private:
    creators _creators;
    std::map<Key, std::shared_ptr<BaseClass> > _available;

};

#endif // GENERIC_FACTORY_HPP
