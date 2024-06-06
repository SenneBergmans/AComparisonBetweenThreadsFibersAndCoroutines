#pragma once

#include <coroutine>
#include <iostream>

//CoroutineHandler example from
//https://en.cppreference.com/w/cpp/language/coroutines
template<typename T>
class CoroutineHandler{
public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;

	struct promise_type {
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
	};

	handle_type coroutine_;

	CoroutineHandler(handle_type h) : coroutine_(h) {}

	// https://stackoverflow.com/questions/62981634/calling-destroy-on-a-coroutine-handle-causes-segfault
	// Add explicit copy constructor
	inline CoroutineHandler(CoroutineHandler&& original) noexcept : coroutine_(original.coroutine_) {
		//Remove the coroutine_ pointer of the original CoroutineHandler instance to avoid double destroy
		original.coroutine_ = nullptr;
	}

	~CoroutineHandler() {
		//Destroy the coroutine if this generator still has a valid pointer to it
		if (coroutine_)	coroutine_.destroy();
	}

	explicit operator bool() {
		//Check if this generator instance still has a valid pointer to the coroutine
		if (!coroutine_) return false;

		callCoroutine();

		return !coroutine_.done();
	}

	T operator()() {
		//Check if this generator instance still has a valid pointer to the coroutine
		if (!coroutine_) return NULL;

		callCoroutine();
		full_ = false;
		return std::move(coroutine_.promise().value_);
	}

	// Remove default copy constructor
	CoroutineHandler(const CoroutineHandler&) = delete;

	// Remove default assignment operator
	CoroutineHandler& operator = (const CoroutineHandler&) = delete;

private:
	bool full_ = false;

	void callCoroutine() {
		if (!full_) {
			coroutine_();
			if (coroutine_.promise().exception_) {
				std::rethrow_exception(coroutine_.promise().exception_);
			}
			full_ = true;
		}
	}
};
