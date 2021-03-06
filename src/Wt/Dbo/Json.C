// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2014 Emweb bvba, Leuven, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "Json"

#include <Wt/WString>
#include <Wt/Dbo/Field_impl.h>

#include "EscapeOStream.h"

namespace Wt {
  namespace Dbo {

JsonSerializer::JsonSerializer(std::ostream& out)
  : out_(out),
    escapeOut_(new Wt::EscapeOStream(out)),
    stringLiteral_(new Wt::EscapeOStream(*escapeOut_)),
    first_(true),
    session_(NULL)
{
  stringLiteral_->pushEscape(Wt::EscapeOStream::JsStringLiteralDQuote);
}

JsonSerializer::~JsonSerializer() {
  delete escapeOut_;
  delete stringLiteral_;
}

void JsonSerializer::act(FieldRef<std::string> field) {
  writeFieldName(field.name());
  fastJsStringLiteral(field.value());
}

void JsonSerializer::act(FieldRef<int> field) {
  writeFieldName(field.name());
  *escapeOut_ << field.value();
}

void JsonSerializer::act(FieldRef<long long> field) {
  writeFieldName(field.name());
  *escapeOut_ << field.value();
}

void JsonSerializer::act(FieldRef<bool> field) {
  writeFieldName(field.name());
  if (field.value())
    *escapeOut_ << "true";
  else
    *escapeOut_ << "false";
}

void JsonSerializer::fastJsStringLiteral(const std::string &s) {
  *escapeOut_ << '"';
  escapeOut_->append(s, *stringLiteral_);
  *escapeOut_ << '"';
}

void JsonSerializer::out(char t) {
  *escapeOut_ << t;
}
void JsonSerializer::out(const char *t) {
  escapeOut_->append(t, strlen(t));
}
void JsonSerializer::out(int t) {
  *escapeOut_ << t;
}
void JsonSerializer::out(long long t) {
  *escapeOut_ << t;
}

void JsonSerializer::writeFieldName(const std::string& fieldName) {
  if (!first_)
    out(',');
  else
    first_ = false;
  fastJsStringLiteral(fieldName);
  out(':');
}

  }
}
