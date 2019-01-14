# Implementation of better performing Map than std::map in C++11

- Implemented a container class template named Map, similar to the std::map class from the C++ Standard Library. Such containers implement key-value pairs, where key and value may be any types, including class types. <br/>
- Implemented key lookup, insertion and deletion strictly in O(lg(N)). <br/>
- From functionality perspective newly implemented Map is targeted to be exactly similar to the std::Map. <br/>
- Std::Map is implemented using balanced binary tree, the goal of this project was to implement the Map using randomized data structure called skip list which if scaled beats the standard Map(in terms of time and space complexity). <br/>
- Insertion and deletion in this implementation is 30-40% faster than std::map.
