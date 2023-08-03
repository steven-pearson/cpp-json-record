//
//  Property.h
//  MixSwiftAndCPP
//
//  Created by Steve Pearson  on 16/06/2019.
//  Copyright © 2019 Steve Pearson . All rights reserved.
//

#ifndef __Property_h
#define __Property_h

#ifdef _MSC_VER
#pragma once
#endif

#include "Event.h"
// LCOV_EXCL_START
#include <utility>
#include <assert.h>
#include <string>
#include <functional>
// LCOV_EXCL_STOP

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

namespace taskly {

    class INotifyPropertyChange {
    public:
        EventBase<NotifyPropertyChangingEventArgs> Changing;
        EventBase<NotifyPropertyChangedEventArgs> Changed;

    public:
        virtual void OnChanging(NotifyPropertyChangingEventArgs& args) const {
            Changing.Notify(args);
        }

        virtual void OnChanged(NotifyPropertyChangedEventArgs& args) const {
            Changed.Notify(args);
        }
    };

    class PropertyBase {
    public:
        PropertyBase(const std::string& name) : _name(name) {
        }

        const std::string& name() const {
            return _name;
        }
        
    private:
        const std::string _name;
    };

    template <typename TValueType>
    class Property : public PropertyBase {

    public:
        typedef std::function<TValueType()> Getter;
        typedef std::function<void(TValueType)> Setter;

        Property(const char* name, INotifyPropertyChange* container, Getter getter, Setter setter = NULL) : PropertyBase(name) {
            _container = container;
            _set = setter;
            _get = getter;
        }

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4181 )
#endif
        //-- Overload the = operator to set the value using the set member --
        void operator =(TValueType value) {
            if (_set != NULL) {
                bool propertyChanging = true;
                TValueType oldValue = _get();

                propertyChanging = (oldValue != value);

                if (propertyChanging && OnChanging(oldValue)) {
                    _set(value);
                    OnChanged(value);
                }
            }
        }
#ifdef _MSC_VER
#pragma warning (pop)
#endif
        //-- Cast the property class to the internal type --
        operator TValueType() {
            try {
                return _get != NULL ? _get() : DefaultGetter();
            }
            catch(...) {
                return DefaultGetter();
            }
        }
        
        const TValueType DefaultGetter() const {
            return TValueType(NULL);
        }
        
        EventBase<PropertyChangingEventArgs<TValueType>> Changing;
        
        EventBase<PropertyChangedEventArgs<TValueType>> Changed;
        
        const bool OnChanging(const TValueType &proposedValue) {
            if (_container) {
                NotifyPropertyChangingEventArgs args(name());
                _container->OnChanging(args);
                if (args.cancelled()) {
                    return false;
                }
            }
            PropertyChangingEventArgs<TValueType> notifyArgs(proposedValue, name());
            return Changing.Notify(notifyArgs);
        }
        
        const bool OnChanged(const TValueType &newValue) {
            PropertyChangedEventArgs<TValueType> args(newValue, name());
            Changed.Notify(args);
            if (_container) {
                NotifyPropertyChangedEventArgs notifyArgs(name());
                _container->OnChanged(notifyArgs);
            }
            return true;
        }

    protected:
        INotifyPropertyChange* _container;
        
    private:
        Setter _set;
        Getter _get;
    };
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
#endif
