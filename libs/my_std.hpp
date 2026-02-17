#pragma once

namespace MyStd {

template<typename T>
struct RemoveReference
{ using type = T; };

template<typename T>
struct RemoveReference<T&>
{ using type = T; };

template<typename T>
struct RemoveReference<T&&>
{ using type = T; };

template<typename T>
constexpr typename RemoveReference<T>::type&& Move(T&& val) noexcept {
    return static_cast<typename RemoveReference<T>::type&&>(val);
}

template<typename T>
constexpr T&& Forward(T&& t) noexcept {
    return static_cast<T&&>(t);
}

};
