////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019-2021 Vladislav Trifochkin
//
// This file is part of [pfs-common](https://github.com/semenovf/pfs-common) library.
//
// Changelog:
//      2019.12.19 Initial version (inhereted from https://github.com/semenovf/pfs)
//      2021.04.25 Moved from pfs-modulus into pfs-common
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "type_traits.hpp"
#include <functional>
#include <list>
#include <mutex>
#include <cassert>

namespace pfs {

template <typename ...Args>
class emitter
{
    using detector_list = std::list<std::function<void(Args...)>>;
    using detector_iterator = typename detector_list::iterator;

    detector_list _detectors;

public:
    using iterator = detector_iterator;

public:
    /**
     * Connect detector defined as ordinary function
     */

    //iterator connect (void (* f) (Args...))
    template <typename F
        //, typename = typename std::enable_if<is_function_pointer<F>::value, F>::type
        , typename = typename std::enable_if<std::is_same<void (*) (Args...), F>::value, F>::type>
    iterator connect (F f)
    {
        _detectors.emplace_back(f);
        return --_detectors.end();
    }

    iterator connect (std::function<void(Args...)> && f)
    {
        _detectors.emplace_back(f);
        return --_detectors.end();
    }

    /**
     * Connect detector defined as member function
     */
    template <typename Class>
    iterator connect (Class & c, void (Class::*f) (Args...))
    {
        _detectors.emplace_back([& c, f] (Args... args) { (c.*f)(args...); });
        return --_detectors.end();
    }

    /**
     * Disconnect detector specified by position @a pos (previously returned
     * by connect() call)
     */
    void disconnect (iterator pos)
    {
        _detectors.erase(pos);
    }

    /**
     * Disconnect all detectors connected to this emitter
     */
    void disconnect_all ()
    {
        _detectors.clear();
    }

    void operator () (Args... args)
    {
        for (auto & f: _detectors) {
            f(args...);
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
// emitter_mt
////////////////////////////////////////////////////////////////////////////////
template <typename ...Args>
class emitter_mt: protected emitter<Args...>
{
    using base_class = emitter<Args...>;
    using mutex_type = std::mutex;

    mutex_type _mtx;

public:
    using iterator = typename base_class::iterator;

public:
    // Apply base class constructors
    using base_class::base_class;

    template <typename F
        , typename = typename std::enable_if<std::is_same<void (*) (Args...), F>::value, F>::type>
    iterator connect (F f)
    {
        std::unique_lock<mutex_type> locker{_mtx};
        return base_class::template connect<F>(f);
    }

    iterator connect (std::function<void(Args...)> && f)
    {
        std::unique_lock<mutex_type> locker{_mtx};
        return base_class::connect(std::forward<std::function<void(Args...)>>(f));
    }

    template <typename Class>
    iterator connect (Class & c, void (Class::*f) (Args...))
    {
        std::unique_lock<mutex_type> locker{_mtx};
        return base_class::template connect<Class>(c, f);
    }

    /**
     * Disconnect detector specified by position @a pos (previously returned
     * by connect() call)
     */
    void disconnect (iterator pos)
    {
        std::unique_lock<mutex_type> locker{_mtx};
        base_class::disconnect(pos);
    }

    /**
     * Disconnect all detectors connected to this emitter
     */
    void disconnect_all ()
    {
        std::unique_lock<mutex_type> locker{_mtx};
        base_class::disconnect_all();
    }

    void operator () (Args... args)
    {
        std::unique_lock<mutex_type> locker{_mtx};
        base_class::operator()(args...);
    }
};

} // pfs