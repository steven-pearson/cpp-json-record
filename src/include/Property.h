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

#include "Variant.h"

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

namespace taskly {

    typedef EventBase<PropertyChangingEventArgs> PropertyChanging;
    typedef EventBase<PropertyChangedEventArgs> PropertyChanged;

    class INotifyPropertyChange {
    public:
        PropertyChanging Changing;
        PropertyChanged Changed;

    public:
        virtual void OnChanging(PropertyChangingEventArgs& args) const {
            Changing.Notify(args);
        }

        virtual void OnChanged(PropertyChangedEventArgs& args) const {
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

    class Property : public PropertyBase {

    private:
        const Variant _defaultValue;

    public:
        typedef Variant&(*Getter)();
        typedef void (*Setter)(Variant&);

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
        void operator =(Variant value) {
            if (_set != NULL) {
                bool propertyChanging = true;
                auto oldValue = _get();

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
        operator Variant() {
            try {
                return _get != NULL ? _get() : DefaultGetter();
            }
            catch(...) {
                return DefaultGetter();
            }
        }
        
        const Variant& DefaultGetter() const {
            return _defaultValue;
        }
        
        PropertyChanging Changing;
        
        PropertyChanged Changed;
        
        const bool OnChanging(const Variant &proposedValue) {
            PropertyChangingEventArgs args(proposedValue, name());
            if (_container) {
                _container->OnChanging(args);
                if (args.cancelled()) {
                    return false;
                }
            }
            return Changing.Notify(args);
        }
        
        const bool OnChanged(const Variant &newValue) {
            PropertyChangedEventArgs args(newValue, name());
            Changed.Notify(args);
            if (_container) {
                _container->OnChanged(args);
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
