#include<iostream>

template<typename Head,typename ...Tail>
void printHelper(Head head,Tail ... tail){
	std::cout << head;
	if constexpr (sizeof...(tail) > 0) {
		std::cout << ",";
		printHelper(tail...);
	}
	else {
		std::cout << "\n";
	}
}

void printHelper() {
	std::cout << "\n";
}

template<auto ... value>
struct list {
	static void print() {
		printHelper(value...);
	}
};

template<typename  List, auto data>
struct push_back;

template<auto ...value, auto data>
struct push_back<list<value...>, data> {
	using type = list<value..., data>;
};

template<typename List, auto data>
using push_back_t = typename push_back<List, data>::type;

template<typename List, auto data>
struct push_front;

template<auto ...value, auto data>
struct push_front<list<value...>, data> {
	using type = list<data, value...>;
};

template<typename List,auto data >
using push_front_t = typename push_front<List,data>::type;

template<template<auto> class Pred, typename List>
class filter;

template< template<auto> class Pred, typename List>
using  filter_t =typename filter<Pred, List>::type;

template<template<auto>class Pred>
class filter<Pred, list<>> {
public:
	using type = list<>;
};

template<template<auto>class Pred, auto head, auto ...tails>
class filter<Pred, list<head, tails...>> {
	using rest = filter_t<Pred, list<tails...>>;
public:
	using type = std::conditional_t<Pred<head>::value,
		rest,
		push_front_t<rest,head>>;
};


template<typename List1,typename List2>
struct concat;

template<auto ...v1,auto ...v2>
struct concat<list<v1...>, list<v2...>> {
	using type = list<v1..., v2...>;
};

template<typename List1, typename List2>
using concat_t = typename concat<List1, List2>::type;

template<typename T>
struct Qsort;

template<typename List>
using Qsort_t = typename Qsort<List>::type;

template<>
struct Qsort<list<>> {
	using type = list<>;
};

template<auto head,auto ...tails>
struct Qsort<list<head,tails...>> {
private:
	template<auto v>
	struct filter_larger {
		static constexpr bool value = v >= head;
	};

	template<auto v>
	struct filter_less {
		static constexpr bool value = v < head;
	};
public:
	using type =concat_t<Qsort_t<filter_t<filter_larger, list<tails...>>>,
		concat_t<list<head>, Qsort_t<filter_t<filter_less, list<tails...>>>>>;
};


int main() {
	using input = list<4, 3, 1, 7, 6, 0>;
	using input2 = list<6, 2, 8, 22, -5, 7>;
	using output = Qsort_t<concat_t<input, input2>>;
	output::print();
}