#ifndef __variant_h_
#define __variant_h_

namespace taskly {

  typedef enum tagVARTYPE {
    vtEmpty,
    vtLong,
    vtByte,
    vtInt,
    vtFloat,
    vtDouble,
    vtBool
  } VARTYPE;

  typedef struct tagVARIANT {
    VARTYPE type = vtEmpty;
    union {
      long longVal;
      unsigned char byteVal;
      int intVal;
      float floatVal;
      double doubleVal;
      bool boolVal;
    } store;
  } VARIANT;

  class Variant {
  public:
    Variant();
    Variant(const Variant& source);
    ~Variant();

    const VARTYPE type() const;

    void clear();

#pragma region // Variant operators
    bool operator !=(const Variant& value) const;
    bool operator ==(const Variant& value) const;
#pragma endregion

#pragma region // Boolean operators
    void operator =(bool boolVal);
    bool operator !=(const bool value) const;
    bool operator ==(const bool value) const;
#pragma endregion

#pragma region // Double operators
    void operator =(double doubleVal);
    double operator !=(const double value) const;
    double operator ==(const double value) const;
#pragma endregion

  private:
    bool equalsDouble(const double value) const;
    bool equalsBool(const double value) const;
    bool areEqual(const Variant& value) const;

    VARIANT _data;  
  };
}

#endif