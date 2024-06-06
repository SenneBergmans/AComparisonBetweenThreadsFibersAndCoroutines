---
layout: post
title:  "Usage - Stackless Coroutines - CoroutineHandler"
date:   2024-05-05 12:00:00 +0200
categories: usage coroutines stackless_coroutines
permalink: "/usage/coroutines/stackless/CoroutineHandler"
description: "CoroutineHandler"
---
This page explains the inner workings of the custom-written CoroutineHandler class that interacts with the handle of stackless coroutines.
The information presented on this page is based on the information provided by the cppreference documentation [1] and information from the Asymmetric Transfer page written by Lewis Baker [2].
This page aims to explain the CoroutineHandler so its purpose and use throughout the paper are clear.
This page explains the functionality used in a simple way.
A more detailed explanation can be found in [1] and [2].
The CoroutineHandler class shown is a renamed copy of the Generator class provided by cppreference [1].

The CoroutineHandler class contains a mix of custom functions that simplify the use of coroutines, as well as some functions that are required to be able to use coroutines.
The main purpose of the CoroutineHandler class is to interact with the coroutine by calling it and checking the bool operator.
The promise_type struct defines the behavior of the coroutine.
On the page 'Understanding the promise type' [3], Lewis Baker explains the details of the promise_type struct.
The promise_type struct will be explained first using the following code:

{% highlight c++ %}
struct promise_type{
    T value_;
    std::exception_ptr exception_;
    
    CoroutineHandler get_return_object() {
        return CoroutineHandler(handle_type::from_promise(*this));
    }
    
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { exception_ = std::current_exception(); }
    
    template<std::convertible_to<T> From>
    std::suspend_always yield_value(From&& from) {
        value_ = std::forward<From>(from);
        return {};
    }
    void return_void() {}
    // void return_value(T value) { result = std::move(value); } //optional in place of return_void

    void* operator new(std::size_t n) noexcept {
        if (void* mem = std::malloc(n)) {
            return mem;
        }
        return nullptr;
    }    
};
{% endhighlight %}

The promise_type has two variables: value_ and exception_.
These variables are not required but are useful in most circumstances.
The value_ variable can store the values yielded by the coroutine.
The exception_ can save any unhandled exception the coroutine comes across.
The main CoroutineHandler class can access these variables and pass them on as needed.

The function get_return_object creates an instance of the CoroutineHandler class and returns it to the caller.
The function is called when the coroutine gets created and will create the CoroutineHandler that the programmer will save.

The functions initial_suspend, final_suspend, and yield_value have a particular return type.
Their return type can either be std::suspend_always or std::suspend_never.
When these functions have return type std::suspend_always, the coroutine will suspend after executing the function.
When these functions have return type std::suspend_never, the coroutine will not suspend.
Choosing between always and never suspending on the initial_suspend function can be compared to choosing for the behavior of a push-type or pull-type coroutine while using Boost Coroutines, as this change will determine when the coroutine first runs. The initial-suspend point and final-suspend point sections of [3] explain this in more detail. 

The initial_suspend function itself will be called when the coroutine is created.
The final_suspend function will be called when the coroutine co_returns or finishes. 
The yield_value function will be called when the coroutine co_yields.
In the example code, the yield_value function uses a template.
This template allows the yield to accept any value convertible to the right type instead of only that type. It is optional and can be left out.

Return_void or return_value are called when the coroutine returns.
The promise_type has to contain one or the other. Both cannot be used simultaneously.
If the return_void type is used, co_return does not expect any parameters and will call the return_void function.
The return_value function can be used if co_return should return a value, which is added as a parameter to co_return.

It is possible to overwrite the new operator of the promise_type.
This operator will be called to allocate the required memory for coroutines.
The function can also be left out, in which case the default new operator for coroutines will be used.

Now that the promise_type has defined the behavior of the coroutine, the CoroutineHandler class can interact with it.
The class contains the following code: 

{% highlight c++ %}
template<typename T> //optional
class CoroutineHandler {
public:
    struct promise_type; //forward declaration to use in shorthand
    using handle_type = std::coroutine_handle<promise_type>;
    struct promise_type{}; //Implementation shown below

    handle_type coroutine_;
    //Constructor used when creating the coroutine
    CoroutineHandler(handle_type h) : coroutine_(h) {}

    //Move constructor
    CoroutineHandler(CoroutineHandler&& original) noexcept : coroutine_(original.coroutine_), full_(original.full_) {
        original.coroutine_ = nullptr;
    }

    //Destructor
    ~CoroutineHandler() {
        if (coroutine_) coroutine_.destroy();
    }

    //Remove default copy constructor
    CoroutineHandler(const CoroutineHandler&) = delete;

    //Remove default assignment operator
    CoroutineHandler& operator = (const CoroutineHandler&) = delete;

    explicit operator bool() {
        if (!coroutine_) return false;
        callCoroutine();
        return !coroutine_.done();
    }
    
    T operator()() {
        if (!coroutine_) return NULL;
        callCoroutine();
        callAhead_ = false;
        return std::move(coroutine_.promise().value_);
    }

private:
    bool callAhead_ = false;
    void callCoroutine() {
        if (!coroutine.done() && !full_) {
            coroutine_();
            if (coroutine_.promise().exception_) {
                std::rethrow_exception(coroutine_.promise().exception_);
            }
            callAhead_ = true;
        }
    }
};
{% endhighlight %}

The shown class uses a template to work with multiple value types.
This template makes the CoroutineHandler usable for more use cases but is not required. 

The CoroutineHandler saves the created coroutine handle, which has type std::coorutine_handle<promise_type> where promise_type is the struct defined above.
For ease of use, a type alias is used in [1] and will be used here, too.

The CoroutineHandler(handle_type h) constructor is used by the get_return_object function from the promise_type to create the CoroutineHandler instance.

In addition to this construct, a move constructor is added.
The default copy constructor and default assignment operator are removed.
These actions allow the coroutine handle to be moved between coroutine handlers and avoid two coroutine handler instances having the same coroutine handle simultaneously.
Two CoroutineHandlers should not have the same coroutine handle simultaneously because both handles are for the same coroutine.
If one of the two CoroutineHandlers gets destroyed, it will destroy the shared coroutine, which can cause errors.
The move constructor will move the coroutine handle to the new instance and set the handle in the original instance to a null pointer.
The destructor checks if this CoroutineHandler still has a coroutine handle and destroys it if that is the case.

The () operator of the CoroutineHandler calls the coroutine, and the boolean operator checks if the coroutine can still be called.
As mentioned in [1], the boolean operator must call the coroutine before it can definitively say whether it can still be called.
This behavior is a result of the coroutine suspending on final_suspend.
A boolean keeping track of the coroutine state solves this problem.
Suppose the bool operator is called while the coroutine is not already ahead.
In that case, the boolean operator will call the coroutine to check if it has finished and then return its answer.
However, calling the coroutine leaves it a single step ahead of the number of times it has been called.
The boolean keeping track of the coroutine will now be set to true, indicating the coroutine is ahead.
This boolean will ensure the coroutine is not called again until an actual call to the CoroutineHandler has been made.
The next call of the () operator of the CoroutineHandler will return the boolean to false, indicating the coroutine is no longer ahead and does nothing else.
If the () operator of the CoroutineHandler is called while the coroutine is not ahead, then the () operator will call the coroutine as expected.
It is good to know how the boolean operator works because while it might not impact the everyday usage of coroutines, it might change to the point where the coroutine is called from the () operator to the boolean operator. It is good to know why this might happen.

### References
[1] "Coroutines (C++ 20)" cppreference.com. Accessed: Apr. 15, 2024. [Online.] Available:
[https://en.cppreference.com/w/cpp/language/coroutines](https://en.cppreference.com/w/cpp/language/coroutines)

[2] L. Baker, "Asymmetric Transfer," lewissbaker.github.io, Accessed: Apr. 19, 2024. [Online]. Available: [https://lewissbaker.github.io](https://lewissbaker.github.io)

[3] L. Baker, "Asymmetric Transfer - C++ Coroutines: Understanding the promise type," lewissbaker.github.io, Accessed: Apr. 19, 2024. [Online]. Available: [https://lewissbaker.github.io/2018/09/05/understanding-the-promise-type](https://lewissbaker.github.io/2018/09/05/understanding-the-promise-type)