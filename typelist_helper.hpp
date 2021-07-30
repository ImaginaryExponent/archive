# pragma once

// Will print full type names when invoked like print<smth>{};
template<class ...>
struct print;


#include <utility>

// Any typelist kind of template will do
template<class ...T>
struct type_list {};

// Main template
template<typename, typename>
struct indexed;

// Indexer. Allows getting type from the list by index and index by type.
template<typename ...Ts, std::size_t ...Is>
struct indexed<type_list<Ts...>, std::index_sequence<Is...>>
{
    // Inherit from a bunch of std::enable_if's. 
    // Each such std::enable_if is a unique-ish class. Those that have
    // the condition satisfied provide a "type" member.
	template<std::size_t Index>
	struct get_type:
		public std::enable_if<Index == Is, Ts>...
	{
	};

    template<std::size_t Index>
	using get_type_t = typename get_type<Index>::type;

    // Same concept as above. Each std::enable_if is a unique-ish class. 
    // Those that have the condition satisfied provide an integral_constant
    // representing type index.
	template<typename T>
	struct get_index:
		public std::enable_if<std::is_same_v<T, Ts>, std::integral_constant<std::size_t, Is>>...
	{
	};

    template<typename T>
	static constexpr auto get_index_v = get_index<T>::value;
};

// Indexer helper providing the index sequence and wrapping a type pack
template<typename ...Ts>
using indexed_t = indexed<type_list<Ts...>, std::index_sequence_for<Ts...>>;

constexpr void test_indexed()
{
	using idx = indexed_t<int, float, bool>;

    static_assert(std::is_same_v<int, idx::get_type_t<0>>);
    static_assert(std::is_same_v<float, idx::get_type_t<1>>);
    static_assert(std::is_same_v<bool, idx::get_type_t<2>>);
		
    static_assert(idx::get_index_v<bool> == 2);
    static_assert(idx::get_index_v<float> == 1);
    static_assert(idx::get_index_v<int> == 0);
}
