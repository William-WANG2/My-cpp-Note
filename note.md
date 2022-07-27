

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

### `data()` Member Function

* `std::string` and `std::vector` have a `data()` member function

* This returns a  pointer to the container's internal memory buffer

*  For `std::string`, this is equivelant to `c_str()`

* useful when working with APIs written in C

  ````c++
  void print(int * arr, size_t size);
  std::vector<int> numbers{1,2,3,4,5};
  print(numbers.data(), numbers.size());
  ````

### `swap()`

````c++
string s1{"Hello"};
string s2{"World"};

s1.swap(s2);

//There is also a non-member version
swap(s1, s2);
````

* This global function has overloads for all built-in and library types.

* Default overload version: If there is no overload for a class, the default implementation will be called.

  ````c++
  //naive shallow copy
  Obj temp = obj1;
  obj1 = obj2;
  obj2 = temp;
  ````

  * Notice that if `std::string` uses this version, it can be quite slow! Since it needs to copy all the data over and over

* ==For `std::string`, the swap actually interchange the encapsulated pointers to the data as well as the integer size!==

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

#### Templates and Code Organization

* Compiler only needs to know the function declaration when the function is called. (to check the types are used correctly)
* For a template function, the compiler must be able to see the definition when the function is called (to generate the instantiated code)
* Most programmers write the template function inside the header files, so it is included automatically
* Some programmers write in a `.inc` file and include it seperately.

#### Class Template

We can create class that works with data of any type

````c++
template<class T>
class Test {
  T data;
  Test(const & T): test(test) {}
}
````

To create an instance of that class

````c++
Test<string> test{"Hello"};

//compiler will generate something like
class Test_bhckbakc {	//instantiated with a unique name
	string data;
  Test(const & string): test(test) {}
}
````

When we create an object of a templated class, C++17 compiler can deduce the parameter type. This is known as **_CTAD: Constructor Template Argument Deduction_**

* Similar to calling a templated function
* The declaration must be initialized
* The compiler deduces the parameter type from the initializer

````c++
vector<int> vec{1, 2, 3}	//C++ 11 declared as vector<int>
vector vec{1, 2, 3}	//C++ 17	deduced as vector<int>
````

#### typename

````c++
template<class T>

template<typename T>	//introduced in C++98, class is a bit confusing as template can be instantiated for built-in type as well as classes
````

### Namespace

* A namespace is used to group together logically related symbols.
  * typically done within libraries
* The C++ standard library defines `std` namespace
* This groups together all the names of the functions, types and variables defined by it.

#### Why?

Large projects may involve several libraries which can lead to name conflicts

#### How to Use

We create a namespace using the namespace keyword and put the symbols we want to declare in this namespace inside the braces.

Every symbol declared in the namespace will have the namespace's name automatically prefixed by the compiler.

````c++
namespace abc {
  class Test;	//defines abc::Test
}
````

If we want to use that symbol outside the namespace, we put `abc::`in front of it.

````c++
abc::Test alphaTest;
````

#### Global Namespace

If a name is not in any namespace, the compiler will assume it to be in the "global namespace". 

* The global namespace has no name
* If we want to specify that the name is in global scope. We can use ==`::Test`==.

#### Namespace Splitting

Namespaces can be split into different parts in the code, or even over different files.

````c++
//In abc's Test.h
namespace abc {
  class Test {
    //...
  };
}

//In abc's Test.cc
namespace abc {
  int Test::do_test(int value) const {
    //...
    //The const member functions are the functions which are declared as constant in the program. The object called by these functions cannot be modified. It is recommended to use const keyword so that accidental changes to object are avoided.
  }
}
````

#### Name Hiding

When a symbol is defined in a namespace, it "hides" any symbols of the same name outside the namespace.

````c++
int x{23};
namespace abc {
  int x{47};
  void func() {
    cout << x << endl;	//abc::x
    cout << ::x << endl;	//global x
  }
}
````

#### Using a Declaration

* A `using`keyword will bring a particular version of symbol into the global namespace.

  ````c++
  using xyz::Test; //"Test" will refer to xyz::Test
  Test //xyz's Test
  ::Test //Global Test
  abc::Test //abc's Test
  ````

* This takes effect from the using declaration until the end of the ==scope==.

* using directive will bring all things into global namespace, e.g., `using namespace std;`

### Function Pointer

A function's executable code is stored in the memory, and we can get a pointer whose value is the start of the code.

````c++
void func(int, int);
auto func_ptr = &func;
//void (*func_ptr)(int, int) = &func;
````

 We can use a type alias for a function pointer's type

````c++
using pfunc = void (*)(int, int);
````

#### Callable Object

`(*func_ptr)(1, 2);`

A function pointer is a "first class object":

* in short, it means there are no restrictions on the object's use. It's the same as any other object.

  A first class object is an entity that can be dynamically created, destroyed, passed to a function, returned as a value, and have all the rights as other variables in the programming language have.

  > Depending on the language, this can imply:
  >
  > - being expressible as an anonymous literal value
  > - being storable in variables
  > - being storable in data structures
  > - having an intrinsic identity (independent of any given name)
  > - being comparable for equality with other entities
  > - being passable as a parameter to a procedure/function
  > - being returnable as the result of a procedure/function
  > - being constructible at runtime
  > - being printable
  > - being readable
  > - being transmissible among distributed processes
  > - being storable outside running processes

* We can pass a function pointer as an argument and return a function pointer from the return statement

#### Pros and Cons

* Usages: wrting callbacks: OS, GUI, event-driven code
* "Raw pointers:" Can be overwritten, null or uninitialized



## C++ String Interface

### Basic Operation

(All works for C-style string and char, except for the assignment)

* Assignment `s1 = s2`, `s1` will have the same data type as `s2`
* Appending `s1+=s2`, `s2`'s data will added at the back of `s1`
* Concatenatation `s1+s2` returns a new string contraining the concatenated information
* Comparison `s1 cmp s2`, where `cmp` are `==, !=, <, > ,<=, >=`

### Compatiblity with C-style string

* `std::string ` has a member function `c_str()`
* return a copy of a C-style string, a pointer to a const char `const char * c_style_str = str.c_str()`
* useful for passing C++ string to a function that takes C string

### Constructor

````c++
string empty;	//empty string, default constructor
string hi{"Hello"};
string triple_x(3, 'x');
string howdy{'h','e','l','l','o'};	//initializer list
string hello1(hi, 1); //ello
string hello2(hi, 1, 3); //ell
````

### Searching

* `find() `returns the first occurrence of its argument in the string. The argument can be a `char`, `std::string` or a C-style string
  * It returns the index of the first match
  * It returns `string::npos` if not find
    * It is a constant static member value with the highest possible value for an element of type size_t.
      * It’s a type which is used to represent the size of objects in bytes and is therefore used as the return type by the **sizeof** [operator](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stddef.h.html). It is guaranteed to be big enough to contain the size of the biggest object the host system can handle. Basically the maximum permissible size is dependent on the compiler; if the compiler is 32 bit then it is simply a typedef(i.e., alias) for **unsigned int** but if the compiler is 64 bit then it would be a typedef for **unsigned long long**. The **size_t** data type is never negative.
        Therefore many C library functions like *malloc, memcpy and strlen* declare their arguments and return type as **size_t**. 
    * It actually means until the end of the [string](https://www.geeksforgeeks.org/string-data-structure/).
    * It is used as the value for a length parameter in the string's member functions.
    * As a return value, it is usually used to indicate no matches.

* `rfind()` returns the last occurrence of its argument. By default, this searches from the end to the begining of the string. We can pass a second argument to change it. 

  ````c++
  string str{"Hello, World"};
  str.rfind(str, 5);	//return 4, search backwards from the element with index 5
  ````

* `find_first_of()` and `find_last_of()` search for the first (last) occurrence of any character from the argument string.

  ````c++
  string vowels{"aeiou"};
  str.find_first_of(vowels);	//1, "Hello, World", e is the first to appear
  ````

* similarly, `find_first_not_of()` and `find_last_not_of()`

### Adding Elements

* `append()` adds its argument at the end of the string

  ````c++
  string hello{'Hello'};
  hello.append(" World"s);`//Hello World
  hello.append("Wow!!!!"s, 3, 2);	//Hello World!!
  ````

* `insert()` 

  ````c++
  string str{"for"};
  str.insert(2, "lde"s);	//folder
  ````

  * insert a substring

    ````c++
    // insert a substring
    string str {"xxx"};
    string str2 {"trombone"};
    str.insert(1, str2, 4, 2);	//xbox
    ````

  * insert a `char` multiple times

    ````c++
    string str3{"cash"};
    str3.insert(1, 3, 'r');	//crrrash
    ````

  * insert at the result of a find call

    ````c++
    auto opos = hello.find('o');
    hello.insert(opo, 2, 'o'); //hello -> hellooo
    ````

  * using iterator: ==this is different from the index, as it will insert before the iterator==

    ````c++
    string str{"ski"};
    auto last = end(str);
    str.insert(last, 2, 'l');	//skill
    ````

    * Iterator Invalidation

      inserting elements may fill up the string's memory buffer. In this case, it will allocate the new buffer and copy the data into the new buffer. However, ==**The saved iterator may still point to the old buffer**==, and they are no longer valid which means using them can cause undefined bahavior.

  ### Removing Elements

  * `erase()`, arguments: $1^{st}$: index of the first element to be erased, $2^{nd}$: number of elements to be erased.

  ````c++
  auto opos = hello.find('e');
  if(opos != string::npos) {
    hello.erase(opos, 2);
  }				
  
  //using iterator
  auto first = begin(hello);
  hello.erase(first); //remove the first element
  
  hello.erase(begin(hello)+1, end(hello)-1); //erase all elements except the first and the last ones
  //range is [ ), not include the right most one
  ````

  * `replace()`, arguments: $1^{st}$: index of the first element to be replaced, $2^{nd}$: the number of characters to be replaced, $3^{rd}$: the characters to replace them

    ````c++
    string str{"Say Hello!"};
    
    auto gdx = str.find("H");
    str.replace(gdx, 5, "Goodbye");	//Say Goodbye!
    
    //iterator
    string str{"Say Goodbye!"};
    str.replace(begin(str), begin(str)+3, "Wave"); //not include begin(str)+3
    ````

  * `assign()`, replace all the characters and replace them. `string str{"Hello"};` `str.assign("Goodbye");`

### Convertion between String and Number

* `to_string`: return ``std::string``, and can take all integers/floating number types. e.g., `string pi{to_string(3.1415926)}`;

* `stoi()` takes `std::string` and returns it as `int`. e.g., `string str{"  314 159"};` ==Result is 314, since it will ignore the leading characers and whenever it encounters a non-numerical type, it will stop==.

  *  Error handling:

    * An optional second argument can be passed which gives the number of characters which are processed.
    * If there is no error, it is equal to the string's size
    * If the conversion is partially successful, this gives the index of the first non-numerical value
    * If it is completely unsuccessful, it throws an error
    * It is assumed to be decimal base, but you can pass a third argument to change it. Base can be in range $1-36$.

    ````c++
    size_t n_processed;
    auto i = stoi(str, &n_processed);
    
    if(n_processed < str.size()) {
      //....
    }
    
    auto j = stoi("2a", nullptr, 16);
    ````

* Similarly, we have `stod()` for floating point numbers, `stoll()` for long long

## Charactor Functions

### Functions inherited from C

* Defined in `<cctype>`

  ````c++
  isdigit(c);	//0-9
  islower(c);	//a-z
  isupper(c);	//A-Z
  isspace(c);	//white space
  ispunct(c);	//?, !, ., ,, etc
  ````

### Miscellaneous

* `toupper()` and `tolower()` 
  * example, linux "Y or y": `if toupper(c) == 'Y`

## Files and Streams

* In C++, a file is represented by a sequence of bytes, identified by a file name. It does not care about where the data is stored and how it is stored.
* File interactions are represented by fstream objects, similar to iostream objects for input/output.
* fstream always access the file sequentially.
  * a sequence of bytes
  * in order
  * of unknown length
  * with no structure
* fstream does not understand file format
* **Operations:**
  * Open: 
    * Connects the fstream object to the file
    * The files become available to the program
  * Read:
    * Data is copied from the file into the program's memory
  * Write:
    * Data is copied from the program's memory into the file
  * Close:
    * Disconnects the fstream object from the file
    * The files is no longer available to the program
  * For each of the operations, the fstream will call the OS's API, and the program will stop and wait the while the operation is performed. Once the operation is done, the API call will return, and the program resumes the execution.
  * You should close the file opened to avoid the possibility of "too many open files error" (OS refuses to allocate more file), though when C++ program terminates, the runtime will close them automatically.

### File Streams

* `ofstream` and `ifstream` like `ostream (cout)` and `istream(cin)`
* in earlier C++, fstream's constructor only takes a C-style string for the file name. e.g., `fstream file{"in.txt"};`. However, in C++ 11 or later, it can take `std::string`. e.g., `string str{"in.txt"};`, `fstream file{str};`
*  can be used as `cin` and `cout`
* If we want to read a line, we can use `getline(fin, text)`, ignoring and discarding the `\n`.
* ==When fstream's destructor is called, the file is automatically destroyed. This will cause any unsaved data to be written to the file. Hence, if the fstream object goes out of the scope once we have finished using it, we do not need to explicitly close it, though it is a good practice to do so.==

### Streams and Bufferings

#### Output buffer

* C++ streams use "buffering" to minimize calls to the operating systems.
* During write operations, data is temporarily held in a memory buffer.
* The size of the buffer is chosed to match the maximum amout of data that the operating system accepts.
* When the buffer is full, the stream will remove the data from the buffer and send it to the operating system.
* This is known as =="flushing"== the output buffer.
* When are stream buffer flushed?
  * For `ostream`, it depends on the terminal configuration.
    * Usually, this is at the end of each line (i.e., if it sees that this is a part of a newline, it will wait for a `\n`)
    * cout is always flushed before the program reads cin 
  * ofstream is only flushed when the buffer is full.
  * ==There is no direct way to flush the **_input stream_**==.
  * **`std::flush`** allows us to control when the stream's buffer is flushed. `cout << str << std::flushed;`will cause all the data in the memory to be immediately sent to its destination.
    * However, this significantly affect the performance. So you should only use it when it is really needed, e.g., output the log.

#### Unbuffered Input and Output

* There are some applications where stream buffer is not suitable when the programmers need more control over how data is transmitted.

* e.g., network applications and communicate with the hardware:

  * Data must be transmitted in packets of a specific size
  * Data may be transmitted at a specific time

* C++ supports lower level of operations on stream

  * These bypass the stream's buffer
  * No formatting of data. e.g., no discarding of white space.

* **`get()`** fetches the next character from an input stream and **`put()`** sends its argument to output stream. They both take ==a `char`== as an argument

* **To handle more than one chars**, we use `read()`, `write()` functions.

  * As we are not using a buffer managed by the stream, we need to provide our own buffer.

    * For `write()`, the buffer will contain all the data we want to send.

    * For `read()`, the buffer must be large enough to store all the data we expect to receive.

    * Both these functions take $1^{st}$: the address of the buffer, $2^{nd}$: the number of characters in the buffer.

      ````c++
      const int filesize{10};
      char filebuf[filesize];
      //...
      ifile.read(filebuf, filesize);
      cout.write(filebuf, filesize);
      ````

#### `gcount()`

* Often, we need to know how much data an input stream has sent us

  * We may need to allocate the memory to process the data.
  * To detect partial or incomplete transfers.

* The `gcount()` function will return the number of characters that are actually received.

  `auto nread = ifile.gcount();	//how many bytes did we receive` 

