# C++ Note

## Review of Basic Concepts



### Declaration and Initialization

#### Universal Intialization

**Advantages:**

1. narrowing conversion is not allowed

   ````c++
   int a = 7.7;	//legal, although compiler may warn
   int b{7.7};	//illegal
   ````

2. Consistent

   ````c++
   vector<int> old_one(4);	//0, 0, 0, 0
   vector<int> old_two(4, 2);	//2, 2, 2, 2
   vector<int> uni{4};	//4
   vector<int> uni{4, 2};	//4, 2
   ````

3. Avoid ambiguity

   ````c++
   //First example
   Test test();	//object creation or function declaration?
   Test test{}; //object creation
   
   //Suppose we have the following
   class Test {}
   int main() {
     Test test();	
   	Test test{}; //compilation error -> test is defined as a function prototype
   }
   
   ````

#### `typedef` and `using` (vec_of_vec's >>)

````c++
//Second example
vector<vector<int>> vec_of_vec	//error -> compiler is confused about >> (shift? cin? ?)
vector<vector<int> > vec_of_vec	//ok
  
//old c++ typedef vector<int> Intvec;
//using Intvec = vector<int>
````

A `typedef` is scoped exactly as the object declaration would have been, so it can be file scoped or local to a block or (in C++) to a namespace or class.

#### NULL and nullptr

* `nullptr` is a literal representation of a null pointer

* `nullptr` is compatible with any pointer type, but cannot be converted to an integer

  ````c++
  void func(int a);
  void func(int * a);
  
  func(nullptr)	//func(int * a) is called
  ````

* traditional `NULL` has value `0`

* Its type is implementation defined

  ````c++
  func(NULL) //Calls func(int *) in clang, func(int) in VC++, does not compile in g++!
  ````

### Class

#### Member Function

````c++
Test test;
test.func(1, 2.0, "abc") //is called as Test::func(test, 1, 2.0, "abc")
````

* Member functions are implemented as global functions
* When it is called on an object, the object is passed as an implicit argument
* pointer to the object is available in the function body as `this`, hence dereferencing it gives access to the object

##### Speicial Member Functions

**Constructor**

````c++
//perform variable initialization when object is created
Test(int i, const string & str): i(i), str(str) {
	//other configuration
}
````

**Copy Constructor**

`````c++
//perform variable initialization when object is created
Test(const Test & other): i(other.i), str(other.str) {
	//other configuration
}
`````

It must use call be reference and the reason is not because of the efficiency. Since if you use copy by value, the it needs to copy that object which itself will use a copy constructor. Therefore, an infinite recursion of calls of copy constructor (const is a famous thing to do)

**Assignment Operator**

Always take a reference to another object of the same class.

Returns a ==non-const== reference to an object with the same class -> hence chaining a = b = c is able to work

````c++
Test & operator=(const Test & other) {
	i = other.i;
	str = other.str;
	return *this;
}
````

**Destructor**

````c++
//prepare object to be released
~Test() {
	//release memory, disconnect with the database, etc
}
````

### Pointers and Memory

````c++
int *p = &a	//stack allocation
int *ptr = new int{36}	//c++ 11, heap allocation will remain allocated until the program release it/the program terminates
````

* The OS restricts the heap memory a program can allocate, if you exceed that amount, the OS may refuse to perform allocation. (Instead of getting a pointer to allocated memory, a special null pointer instead)
* failing to release heap memory -> "memory leak" (the pointer which keeps track of the memory is on stack, but when the scope ends, it does not exist)

````c++
delete p	//delete the memory at p, but pointer p still exists, "dangling pointer", access it can cause undefined behavior
````

### String and Vector

**C-string: string literal**

````c++
const char * str = "Hello world";
````

**C++ string** `std:string`

1. behave like a dynamic heap, but can be used as local variable
2. Contiguous block of memory will be allocated on the heap inside the class, and memory will be released by the destructor.
3. Copy constructor/assignment constructor -> allocating a new block of memory
4. automatically reallocate the extra memory

Think its structure like:

````c++
class std::string {
	char * data; //block of contiguous memory on heap
  size_t n;	//number of elements
}
````

**vector**

similar structure as string (block of contiguous memory on heap)

### Vscode Configuration for Linkage

When we link multiple cpp files, we need to add arguments in `task.json`, e.g. `"other1.cc", "other2,cc"`. 

````javascript
{
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: clang++ build active file",
            "command": "/usr/bin/clang++",
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                "${file}",
                "other1.cc",
              	"other2,cc"
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
            ],
            "options": {
                "cwd": "${fileDirname}"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Task generated by Debugger."
        }
    ],
    "version": "2.0.0",
    "label": "Open Terminal",
    "type": "shell",
    "command": "osascript -e 'tell application \"Terminal\"\ndo script \"echo hello\"\nend tell'",
    "problemMatcher": []
}
````



### Two Dim Array

* It is stored as a single memory block, meaning that it is contiguous in memory

  ````c++
  int ints[2][4] = {1,2,3,4,5,6,7,8} //valid
  int ints[2][4] = {{1,2,3,4},{5,6,7,8}} //better and more clear
  ````

### Numeric Types and Literals

The size of C++ types are implementation dependent

* `char` 8 bits
* `int` $\geq$ 16 bits
* `long` $\geq$ 32 bits and `int`'s length
* `long long ` new in C++ 11, $\geq$ 64 bits and `long`'s length

**Fixed Length Integer**

introduced in C++11 `<cstdint>`

````c++
int8_t
int16_t
int32_t
int64_t
uint8_t
uint16_t
uint32_t
uint64_t
````

**Bases**

````c++
 ()0x2a //hex
052 //octal
0b101010 0B101010 //binary (C++ 14)
````

**Digital Seperators in Numeric Literal** (C++ 14)

````c++
const int one_million = 1'000'000';
double pi = 3.141'592;
````

**Default Type**

````c++
3.141592;	//literal default double
//integers are by default int -> too big then long -> too too big then long long
//We can add suffix to change the default
3.141592f; //float
123456ULL	//unsigned long long
````

### String Literal

#### C-style String Literal

**An array of const char** `const char * cca = "Hello World!"`;

Very limited of operations and since it is an array, it is only compatibel with arrays of the same length

#### C++ 14 String Literal

* `std::string` literals

* ````c++
  using namespace std::literals;
  
  string str = "Hello, World"s;	//std::string
  ````

* Support all `std::string `operations

  ````c++
  cout << "Hello"s + "World"s;
  ````

* Can be used anywhere expecting an std::string

#### Escape Characters and Raw String Literals

````c++
// to write <a href="file">C:\"Programs Files"\</a>\n
"<a href=\"file\">C:\\\"Programs Files\"\\</a>\\n"
````

C++ 11 provides Raw String Literals.

````c++
string str = R"(<a href="file">C:\"Programs Files"\</a>\n)";
//if string contains (), add delimiter
string str = R"x(<a href="file">C:\"Programs Files (arm)"\</a>\n)x";
````

### Casting

* A cast performs an explicit conversion
* It can:
  * convert an expression to a defferent type
  * const to non-const
  * convert data in buffer to an untyped binary data (suppose you try to send some data alongside the network which knows nothing about C++)
  * convert pointer to base class to pointer to derived (inheritance)

C++ 98 casting `xyz_case<Type>(data)`

**static_cast** is used to convert an expression to a different type **(compilation time)**

````c++
cout << static_cast<char>(c) << endl;	//c is int
````

**const_case** is used to change from const to non-cosnt, which may be used to call functions that aren't const-correct (badly written) **(compilation time)**

**reinterpret_case** is used to convert data in a buffer to untyped binary data. (mainly used in low level work (commmunication with OS, hardware, etc)) **(compilation time)**

**dynamic_cast** is used to convert a pointer to a base case to pointer to derived (inheritance) **(run time)**

### `auto` Keyword

* In C, "auto" was originally used to specify that a variable should be created on stack

* in C++, it is used to denote that the compiler should deduce the type from the **variable's initial value**

  ````c++
  auto i{42};	//int
  auto str1 = "Hello";	//const char[] str1 = 'Hello';
  auto str2 = "Hello"s;	//string str2 = "Hello"s;
  ````

**Why?**

* Simplyfy the syntax
* Sometimes it is not easy to work out the type/type it corrrectly
* In modern C++, there are even situations that we do not know what the type is! (e.g. compiler generated class)

==Warnings==

`auto` will only gives the underlying type, but `const`, reference `&` are ignored!

````c++
const int & x{6}; // x is a reference to a const int
// side note: int & const x = a // x is a const reference to an int a. Since references refer to the same object since their creation, they are always constant. The const is redundant.
auto y = x;

y = 7; //allowed!
````

if we need them, we must add them ourselves

````c++
const auto & y = x;
````

### Iterator

#### Const Iterator

* If we want to prevent the loop from modifying the string, we should use const_iterator. e.g., `string::const_iterator cit;`

* Or you must use it for const container

#### Reverse Iterator

reverse from the last element to the front. e.g., `string::reverse_iterator rit;`

Modern C++ has const begin and end iterator.

````c++
auto it = str.cbegin(); 
auto it = str.crend();
````

**begin() and end() global functions which are more readable**

````c++
//C++ 11
for (auto it = begin(str); it!=end(arr); ++it) {
  //code
}
//also works for built-in type
int arr[] = {1,2,3,4,5};
auto it = begin(arr);
//C++ 14
cbegin();
rbegin();
crend();
...
````

#### Range For-loops

````c++
for (auto el: vec) {
  //code
}
//equivalent to 
for (auto it = begin(vec); it!=end(vec); it++) {
  //code
}
````

==`el`== is a copy which means modifying el will not affect the elements in `vec`

In order to modify the element in `vec`, use `auto & el:vec`

#### Iterator Arithmetic

* similar to pointers
* `auto second = begin(str) + 1 //move to the right by 1 element`
* `auto mid = begin(str) + (begin(str)+end(str))/2 //move to the middle`

#### Iterator Arithmetic Function

* next() takes an iterator and returns the following iterator. `auto second = next(begin(str));`

* prev() takes an iterator and returns the previous iterator. `auto last = prev(end(str));`

* distance() returns the number of steps needed to go from the first argument to the second argument

* advance() moves an iterator by its second argument

  ````c++
  auto mid = begin(str);
  advance(mid, distance(begin(str), end(str)/2); //return the iterator to the mid
  ````

  

### Initializator in if/for/switch Statement

````c++
auto iter = begin(vec);
if(iter != end(vec)) {} //check if container is empty or not

if(auto iter = begin(vec); iter!=end(vec)) {} //	C++17

switch(const char c = get_next_char();c) {} //local to the scope of switch
````

`iter` is similar to the for loop initializator which is local to the scope (including the `else` block)

==C++ 17== provides a `fallthrough`attribute which is used with an empty statement for case and the compiler will not give warning to it. It indicates that the `break`statement is intentionally omitted.

````c++
switch(c) {
  case 1:
    [[fallthrough]];
  case 3:
    [[fallthrough]];
  case 5:
    //some statements
  default:
    break;
}
````

### Template

#### Why?

* Write codes which are functionally the same but for different types of data. **(Known as Generic Porgramming)**

  * vector of int

  * vector of chars

  * vector of string

#### Template Instantiation

* When we use the template with data of a particular type, the compiler will generate the code for that specific type.
  * e.g., `vector<int>`will cause the compiler to define a class which that is a vector of int (plugin `int`, hence, different class for different vector type)
  * The compiler will insert the source code for the class definition into the translation unit. The translation unit is basically a source code file as it is seen by the compiler.
  * This code will then be compiled as part of the program
* It happens automatically when the compiler sees something like `vector<int>`
* For this to be possible, the compiler must be able to see the full definition of the vector template class in the transilation unit, meaning that put it into one of the header files.

#### Writing Template

```c++
template <class T>
T max(const T & t1, const T & t2) {
  if(t1 > t2) {
    return t1;
  }
  else {
    return t2;
  }
}
```

* T is the 'dummy' type to show the compiler what the code is like. official name for 'dummy type' is **template parameter**
* A template can either be ==class template== or ==function template==

#### Using Template

* When we call Max(), the compiler will instantiate the function from the template code, with T replaced by the type of arguments.
* `cout << Max(7.1, 4.9) << endl;`
* The compiler deduces the needed argument type should be `double`.
* The compiler will generate a definition of the Max function, in which `T ` is replaced by `double`.







