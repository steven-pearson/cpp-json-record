//
//  Event.h
//  MixSwiftAndCPP
//
//  Created by Steve Pearson  on 16/06/2019.
//  Copyright © 2019 Steve Pearson . All rights reserved.
//

#ifndef __Event_h
#define __Event_h

#ifdef _MSC_VER
#pragma once
#endif

// LCOV_EXCL_START
#include <map>
#include <string>
#include <exception>
#include <functional>
#include <iostream>
// LCOV_EXCL_STOP

namespace taskly {
    typedef size_t EventHandlerID;
    
    struct event_exception: std::exception {
        event_exception (const std::string& what) { _what = what; }
        ~event_exception () throw () {}
        
        const char*
        what () const throw () { return _what.c_str(); }
        
    private:
        std::string _what;
    };
    
    class EventArgs {
    public:
        EventArgs(bool cancellable = false) : _cancellable(cancellable) {
            _cancelled = false;
        }
        
        const bool cancellable() const { return _cancellable; }
        
        const bool cancelled() const { return _cancelled; }
        
        void cancel(const std::string& failureCode = "", const std::string& failureReason = "") {
            if (_cancellable && !_cancelled) {
                _cancelled = true;
                _failureCode = failureCode;
                _failureReason = failureReason;
            }
        }
        
        const std::string& failureReason() const { return _failureReason; }
        const std::string& failureCode() const { return _failureCode; }
        
    private:
        std::string _failureCode;
        std::string _failureReason;
        bool _cancelled = false;
        const bool _cancellable;
    };

    class NotifyPropertyChangingEventArgs : public EventArgs{
        
    public:
        NotifyPropertyChangingEventArgs(const std::string& name) :
            EventArgs(true),
            _name(name) {
        }
        
        const std::string& name() const { return _name; }
        
    private:
        const std::string& _name;
    };

    class NotifyPropertyChangedEventArgs : public EventArgs {
        
    public:
        NotifyPropertyChangedEventArgs(const std::string& name) : _name(name) {
        }
        
        const std::string& name() const { return _name; }
        
    private:
        const std::string& _name;
    };

    template<typename ParamT>
    class PropertyChangingEventArgs : public EventArgs {
        
    public:
        PropertyChangingEventArgs(const ParamT& proposedValue, const std::string& name) :
            EventArgs(true),
            _name(name),
            _proposedValue(proposedValue) {
        }
        
        const std::string& name() const { return _name; }
        const ParamT& proposedValue() const { return _proposedValue; }

    private:
        const std::string& _name;
        const ParamT& _proposedValue;
    };

    template<typename ParamT>
    class PropertyChangedEventArgs : public EventArgs {
        
    public:
        PropertyChangedEventArgs(const ParamT& newValue, const std::string& name) :
            _name(name),
            _newValue(newValue) {
        }
        
        const std::string& name() const { return _name; }
        const ParamT& newValue() const { return _newValue; }

    private:
        const std::string& _name;
        const ParamT& _newValue;
    };

    template<typename ParamT>
    class EventHandlerBase {
    public:
        virtual void Notify(EventArgs& eventArgs) const = 0;
        virtual ~EventHandlerBase() {};
    };

    template<typename EventArgsT>
    class EventHandler : public EventHandlerBase<EventArgsT> {
        void *_context;
        std::function<void(EventArgsT&)> _eventHandler;
        
    public:
        EventHandler(void *context, std::function<void(EventArgsT&)> eventHandler)
        : _context(context), _eventHandler(eventHandler)
        {
        }

        void Notify(EventArgsT& eventArgs) const {
            return (_eventHandler)();
        }
    };

    template<typename EventArgsT>
    class EventBase {
        typedef std::map <EventHandlerID, std::function<void(EventArgsT&)>> HandlerMap;
        HandlerMap _eventHandlers;
        EventHandlerID _count;
        
    public:
        EventBase()
            : _count(0)
        {
        }
        
        ~EventBase() {
            for (size_t i = _eventHandlers.size() - 1; i < 0; i--) {
            _eventHandlers.erase(i);
            }
        }
        
        EventHandlerID Subscribe(std::function<void(EventArgsT&)> const &block) {
            _eventHandlers[_count++] = block;
            return _count-1;
        }
        
        void Unsubscribe(EventHandlerID handlerId) {
            _eventHandlers.erase(handlerId);
        }
        
        const bool Notify(EventArgsT& eventArgs) const {
            for (size_t index=0; index < _count && index < _eventHandlers.size(); index++) {
                _eventHandlers.find(index)->second(eventArgs);
                if (eventArgs.cancelled()) {
                    std::cout << "Cancelled event";
                    return false;
                }
            }
            return true;
        }
    };
}

#endif

