#pragma once

#include "DynamicInterval.h"
#include <vector>

template<typename T, IntervalWrapModes Mode = IntervalWrapModes::Clamp, typename Allocator = typename std::vector<T>::allocator_type>
class VectorWithIntervalRead : public std::vector<T, Allocator>
{
public:
    using ContainerType = std::vector<T, Allocator>;
    using IntervalType = DynamicInterval<typename ContainerType::size_type>;

    constexpr VectorWithIntervalRead() = default;

    explicit VectorWithIntervalRead( const Allocator& alloc ) noexcept : ContainerType(alloc) {}
    explicit VectorWithIntervalRead( typename ContainerType::size_type count, const T& value, const Allocator& alloc = Allocator()) : ContainerType(count, value, alloc) {rescaleIndex();}
    explicit VectorWithIntervalRead( typename ContainerType::size_type count, const Allocator& alloc = Allocator() ) : ContainerType(count, alloc) {rescaleIndex();}
    template< class InputIt >
    VectorWithIntervalRead( InputIt first, InputIt last, const Allocator& alloc = Allocator() ) : ContainerType(first, last, alloc) {rescaleIndex();}

    VectorWithIntervalRead(const ContainerType& other) : ContainerType(other, std::allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())) {rescaleIndex();}
    VectorWithIntervalRead( const ContainerType& other, const Allocator& alloc) : ContainerType(other, alloc) {rescaleIndex();}
    VectorWithIntervalRead( ContainerType&& other ) noexcept : ContainerType(other) {rescaleIndex();}
    VectorWithIntervalRead( ContainerType&& other, const Allocator& alloc ) : ContainerType(other, alloc) {rescaleIndex();}
    VectorWithIntervalRead( std::initializer_list<T> init, const Allocator& alloc = Allocator() ) : ContainerType(init, alloc) {rescaleIndex();}

    template<typename U>
    constexpr T& operator[](const DynamicInterval<U>& index) noexcept {
        indexingInterval = index;
        return ContainerType::operator[](indexingInterval.getValue());
    }

    template<typename U>
    constexpr const T& operator[](const DynamicInterval<U>& index) const noexcept {
        indexingInterval = index;
        return ContainerType::operator[](indexingInterval.getValue());
    }

    constexpr T& operator[](const typename ContainerType::size_type& index) noexcept {
        indexingInterval = index;
        return ContainerType::operator[](indexingInterval.getValue());
    }

    constexpr const T& operator[](const typename ContainerType::size_type& index) const noexcept{
        indexingInterval = index;
        return ContainerType::operator[](indexingInterval.getValue());
    }

    template<typename U>
    constexpr T& at(const DynamicInterval<U>& index) noexcept { indexingInterval = index; return ContainerType::at(indexingInterval.getValue()); }

    template<typename U>
    constexpr const T& at(const DynamicInterval<U>& index) const noexcept { indexingInterval = index; return ContainerType::at(indexingInterval.getValue()); }

    constexpr T& at(const typename ContainerType::size_type& index) noexcept { indexingInterval = index; return ContainerType::at(indexingInterval.getValue()); }

    constexpr const T& at(const typename ContainerType::size_type& index) const noexcept { indexingInterval = index; return ContainerType::at(indexingInterval.getValue()); }




    void resize(const typename ContainerType::size_type&& newSize) { ContainerType::resize(std::forward<const size_t>(newSize)); rescaleIndex(); }

    void push_back(T&& newElement) { ContainerType::push_back(std::forward<T>(newElement)); rescaleIndex(); }

    void pop_back() { ContainerType::pop_back(); rescaleIndex(); }

    void emplace_back(T&& newElement) { ContainerType::emplace_back(std::forward<T>(newElement)); rescaleIndex(); }

    typename ContainerType::iterator insert( typename ContainerType::iterator&& pos, T&& value ) { insert(std::forward<decltype(pos)>(pos), std::forward<T>(value)); rescaleIndex(); }

    typename ContainerType::iterator insert( typename ContainerType::const_iterator&& pos, T&& value ) { insert(std::forward<decltype(pos)>(pos), std::forward<T>(value)); rescaleIndex(); }

    typename ContainerType::iterator insert( typename ContainerType::const_iterator&& pos, typename ContainerType::size_type&& count, T&& value ) { insert(std::forward<decltype(pos)>(pos), std::forward<decltype(count)>(count), std::forward<T>(value)); rescaleIndex(); }

    template< class InputIt >
    typename ContainerType::iterator insert( typename ContainerType::const_iterator&& pos, InputIt&& first, InputIt&& last ) { insert(std::forward<decltype(pos)>(pos), std::forward<InputIt>(first), std::forward<InputIt>(last)); rescaleIndex(); }

    typename ContainerType::iterator insert( typename ContainerType::const_iterator&& pos, std::initializer_list<T> ilist ) { insert(std::forward<decltype(pos)>(pos), std::forward<decltype(ilist)>(ilist)); rescaleIndex(); }


    template< class... Args >
    typename ContainerType::iterator emplace(typename ContainerType::const_iterator&& pos, Args&&... args ) { ContainerType::emplace(std::forward<typename ContainerType::const_iterator>(pos), std::forward<Args...>(args...)); rescaleIndex(); }
    
    void assign(const typename ContainerType::size_type&& count, T&& newElement) { ContainerType::assign(std::forward<decltype(count)>(count), std::forward<T>(newElement)); rescaleIndex(); }

    template<typename IteratorType>
    void assign(IteratorType&& first, IteratorType&& last) { ContainerType::assign(std::forward<IteratorType>(first), std::forward<IteratorType>(last)); rescaleIndex(); }

    void assign(const std::initializer_list<T>&& newElement) { ContainerType::assign(std::forward<decltype(newElement)>(newElement)); rescaleIndex(); }

    void erase(typename ContainerType::const_iterator&& iterator) { ContainerType::erase(std::forward<typename ContainerType::const_iterator>(iterator)); rescaleIndex(); }

    void erase(typename ContainerType::const_iterator&& beginIterator, typename ContainerType::const_iterator&& endIterator) { ContainerType::erase(std::forward<typename ContainerType::const_iterator>(beginIterator), std::forward<typename ContainerType::const_iterator>(endIterator)); rescaleIndex(); }

    void clear() noexcept { ContainerType::clear(); rescaleIndex(); }

    void swap(ContainerType&& other) { ContainerType::swap(std::forward<ContainerType>(other)); rescaleIndex(); }
    void swap(VectorWithIntervalRead<T, Mode>&& other) { swap(std::forward<ContainerType>(static_cast<ContainerType>(other))); }
    
private:
    mutable IntervalType indexingInterval{0,ContainerType::size(),0,Mode};

    // each operation that triggers a resize must resize the index
    void rescaleIndex() noexcept { indexingInterval.setEnd(this->size()-1); }
};

template<typename T, typename Allocator = typename std::vector<T>::allocator_type>
using BoundedVector = VectorWithIntervalRead<T, IntervalWrapModes::Clamp, Allocator>;

template<typename T, typename Allocator = typename std::vector<T>::allocator_type>
using PeriodicVector = VectorWithIntervalRead<T, IntervalWrapModes::Wrap, Allocator>;
