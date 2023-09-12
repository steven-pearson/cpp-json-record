#include "include/Variant.h"

namespace taskly {
  Variant::Variant() {
  }

  Variant::Variant(const Variant& source) : _data(source._data) {
  }

  Variant::~Variant() {
  }
  const VARTYPE Variant::type() const {
    return _data.type;
  }

  void Variant::clear() {
    _data.type = vtEmpty;
  }

#pragma region // Variant operators
  bool Variant::operator !=(const Variant& value) const {
    return !areEqual(value);
  }

  bool Variant::operator ==(const Variant& value) const {
    return areEqual(value);
  }
#pragma endregion

#pragma region // Boolean operators
  void Variant::operator =(bool boolVal) {
    _data.store.boolVal = boolVal;
    _data.type = vtBool;
  }

  bool Variant::operator !=(const bool value) const {
    return !equalsBool(value);
  }

  bool Variant::operator ==(const bool value) const {
    return equalsBool(value);
  }
#pragma endregion

#pragma region // Double operators
  void Variant::operator =(double doubleVal) {
    _data.store.doubleVal = doubleVal;
    _data.type = vtDouble;
  }

  double Variant::operator !=(const double value) const {
    return !equalsDouble(value);
  }

  double Variant::operator ==(const double value) const {
    return equalsDouble(value);
  }
#pragma endregion

  bool Variant::equalsDouble(const double value) const {
    return _data.type == vtDouble && _data.store.doubleVal == value;
  }

  bool Variant::equalsBool(const double value) const {
    return _data.type == vtBool && _data.store.boolVal == value;
  }

  bool Variant::areEqual(const Variant& value) const {
    if (_data.type != value._data.type) {
      return false;
    }
    switch (_data.type) {
      case vtLong : return _data.store.longVal == value._data.store.longVal;
      case vtByte : return _data.store.byteVal == value._data.store.byteVal;
      case vtInt : return _data.store.intVal == value._data.store.intVal;
      case vtFloat : return _data.store.floatVal == value._data.store.floatVal;
      case vtDouble : return _data.store.doubleVal == value._data.store.doubleVal;
      case vtBool : return _data.store.boolVal == value._data.store.boolVal;
      case vtEmpty : return true;
    }
    return false;
  }
}
