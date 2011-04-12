// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "entry.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace cloudflare {

namespace {

const ::google::protobuf::Descriptor* ZoneTimeBucket_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ZoneTimeBucket_reflection_ = NULL;
const ::google::protobuf::Descriptor* ZoneTimeBucket_Counter_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ZoneTimeBucket_Counter_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_entry_2eproto() {
  protobuf_AddDesc_entry_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "entry.proto");
  GOOGLE_CHECK(file != NULL);
  ZoneTimeBucket_descriptor_ = file->message_type(0);
  static const int ZoneTimeBucket_offsets_[10] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, map_name_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, classification_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, doctype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, pop_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, psource_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, result_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, db_key_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, db_path_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, kv_map_file_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, map_entry_),
  };
  ZoneTimeBucket_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ZoneTimeBucket_descriptor_,
      ZoneTimeBucket::default_instance_,
      ZoneTimeBucket_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ZoneTimeBucket));
  ZoneTimeBucket_Counter_descriptor_ = ZoneTimeBucket_descriptor_->nested_type(0);
  static const int ZoneTimeBucket_Counter_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket_Counter, key_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket_Counter, value_),
  };
  ZoneTimeBucket_Counter_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ZoneTimeBucket_Counter_descriptor_,
      ZoneTimeBucket_Counter::default_instance_,
      ZoneTimeBucket_Counter_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket_Counter, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ZoneTimeBucket_Counter, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ZoneTimeBucket_Counter));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_entry_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ZoneTimeBucket_descriptor_, &ZoneTimeBucket::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ZoneTimeBucket_Counter_descriptor_, &ZoneTimeBucket_Counter::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_entry_2eproto() {
  delete ZoneTimeBucket::default_instance_;
  delete ZoneTimeBucket_reflection_;
  delete ZoneTimeBucket_Counter::default_instance_;
  delete ZoneTimeBucket_Counter_reflection_;
}

void protobuf_AddDesc_entry_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013entry.proto\022\ncloudflare\"\220\002\n\016ZoneTimeBu"
    "cket\022\020\n\010map_name\030\001 \002(\t\022\026\n\016classification"
    "\030\002 \002(\t\022\017\n\007doctype\030\003 \002(\t\022\013\n\003pop\030\004 \002(\t\022\017\n\007"
    "psource\030\005 \002(\t\022\021\n\tresult_id\030\006 \002(\t\022\016\n\006db_k"
    "ey\030\007 \002(\t\022\017\n\007db_path\030\010 \002(\t\022\023\n\013kv_map_file"
    "\030\t \001(\t\0225\n\tmap_entry\030\n \003(\0132\".cloudflare.Z"
    "oneTimeBucket.Counter\032%\n\007Counter\022\013\n\003key\030"
    "\001 \002(\t\022\r\n\005value\030\002 \002(\003", 300);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "entry.proto", &protobuf_RegisterTypes);
  ZoneTimeBucket::default_instance_ = new ZoneTimeBucket();
  ZoneTimeBucket_Counter::default_instance_ = new ZoneTimeBucket_Counter();
  ZoneTimeBucket::default_instance_->InitAsDefaultInstance();
  ZoneTimeBucket_Counter::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_entry_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_entry_2eproto {
  StaticDescriptorInitializer_entry_2eproto() {
    protobuf_AddDesc_entry_2eproto();
  }
} static_descriptor_initializer_entry_2eproto_;


// ===================================================================

const ::std::string ZoneTimeBucket_Counter::_default_key_;
#ifndef _MSC_VER
const int ZoneTimeBucket_Counter::kKeyFieldNumber;
const int ZoneTimeBucket_Counter::kValueFieldNumber;
#endif  // !_MSC_VER

ZoneTimeBucket_Counter::ZoneTimeBucket_Counter()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ZoneTimeBucket_Counter::InitAsDefaultInstance() {
}

ZoneTimeBucket_Counter::ZoneTimeBucket_Counter(const ZoneTimeBucket_Counter& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ZoneTimeBucket_Counter::SharedCtor() {
  _cached_size_ = 0;
  key_ = const_cast< ::std::string*>(&_default_key_);
  value_ = GOOGLE_LONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ZoneTimeBucket_Counter::~ZoneTimeBucket_Counter() {
  SharedDtor();
}

void ZoneTimeBucket_Counter::SharedDtor() {
  if (key_ != &_default_key_) {
    delete key_;
  }
  if (this != default_instance_) {
  }
}

void ZoneTimeBucket_Counter::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ZoneTimeBucket_Counter::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ZoneTimeBucket_Counter_descriptor_;
}

const ZoneTimeBucket_Counter& ZoneTimeBucket_Counter::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_entry_2eproto();  return *default_instance_;
}

ZoneTimeBucket_Counter* ZoneTimeBucket_Counter::default_instance_ = NULL;

ZoneTimeBucket_Counter* ZoneTimeBucket_Counter::New() const {
  return new ZoneTimeBucket_Counter;
}

void ZoneTimeBucket_Counter::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (key_ != &_default_key_) {
        key_->clear();
      }
    }
    value_ = GOOGLE_LONGLONG(0);
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ZoneTimeBucket_Counter::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string key = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_key()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->key().data(), this->key().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_value;
        break;
      }
      
      // required int64 value = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_value:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &value_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ZoneTimeBucket_Counter::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string key = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->key().data(), this->key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->key(), output);
  }
  
  // required int64 value = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->value(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ZoneTimeBucket_Counter::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string key = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->key().data(), this->key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->key(), target);
  }
  
  // required int64 value = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->value(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ZoneTimeBucket_Counter::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string key = 1;
    if (has_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->key());
    }
    
    // required int64 value = 2;
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->value());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ZoneTimeBucket_Counter::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ZoneTimeBucket_Counter* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ZoneTimeBucket_Counter*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ZoneTimeBucket_Counter::MergeFrom(const ZoneTimeBucket_Counter& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_key(from.key());
    }
    if (from._has_bit(1)) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ZoneTimeBucket_Counter::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ZoneTimeBucket_Counter::CopyFrom(const ZoneTimeBucket_Counter& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ZoneTimeBucket_Counter::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;
  
  return true;
}

void ZoneTimeBucket_Counter::Swap(ZoneTimeBucket_Counter* other) {
  if (other != this) {
    std::swap(key_, other->key_);
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ZoneTimeBucket_Counter::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ZoneTimeBucket_Counter_descriptor_;
  metadata.reflection = ZoneTimeBucket_Counter_reflection_;
  return metadata;
}


// -------------------------------------------------------------------

const ::std::string ZoneTimeBucket::_default_map_name_;
const ::std::string ZoneTimeBucket::_default_classification_;
const ::std::string ZoneTimeBucket::_default_doctype_;
const ::std::string ZoneTimeBucket::_default_pop_;
const ::std::string ZoneTimeBucket::_default_psource_;
const ::std::string ZoneTimeBucket::_default_result_id_;
const ::std::string ZoneTimeBucket::_default_db_key_;
const ::std::string ZoneTimeBucket::_default_db_path_;
const ::std::string ZoneTimeBucket::_default_kv_map_file_;
#ifndef _MSC_VER
const int ZoneTimeBucket::kMapNameFieldNumber;
const int ZoneTimeBucket::kClassificationFieldNumber;
const int ZoneTimeBucket::kDoctypeFieldNumber;
const int ZoneTimeBucket::kPopFieldNumber;
const int ZoneTimeBucket::kPsourceFieldNumber;
const int ZoneTimeBucket::kResultIdFieldNumber;
const int ZoneTimeBucket::kDbKeyFieldNumber;
const int ZoneTimeBucket::kDbPathFieldNumber;
const int ZoneTimeBucket::kKvMapFileFieldNumber;
const int ZoneTimeBucket::kMapEntryFieldNumber;
#endif  // !_MSC_VER

ZoneTimeBucket::ZoneTimeBucket()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ZoneTimeBucket::InitAsDefaultInstance() {
}

ZoneTimeBucket::ZoneTimeBucket(const ZoneTimeBucket& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ZoneTimeBucket::SharedCtor() {
  _cached_size_ = 0;
  map_name_ = const_cast< ::std::string*>(&_default_map_name_);
  classification_ = const_cast< ::std::string*>(&_default_classification_);
  doctype_ = const_cast< ::std::string*>(&_default_doctype_);
  pop_ = const_cast< ::std::string*>(&_default_pop_);
  psource_ = const_cast< ::std::string*>(&_default_psource_);
  result_id_ = const_cast< ::std::string*>(&_default_result_id_);
  db_key_ = const_cast< ::std::string*>(&_default_db_key_);
  db_path_ = const_cast< ::std::string*>(&_default_db_path_);
  kv_map_file_ = const_cast< ::std::string*>(&_default_kv_map_file_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ZoneTimeBucket::~ZoneTimeBucket() {
  SharedDtor();
}

void ZoneTimeBucket::SharedDtor() {
  if (map_name_ != &_default_map_name_) {
    delete map_name_;
  }
  if (classification_ != &_default_classification_) {
    delete classification_;
  }
  if (doctype_ != &_default_doctype_) {
    delete doctype_;
  }
  if (pop_ != &_default_pop_) {
    delete pop_;
  }
  if (psource_ != &_default_psource_) {
    delete psource_;
  }
  if (result_id_ != &_default_result_id_) {
    delete result_id_;
  }
  if (db_key_ != &_default_db_key_) {
    delete db_key_;
  }
  if (db_path_ != &_default_db_path_) {
    delete db_path_;
  }
  if (kv_map_file_ != &_default_kv_map_file_) {
    delete kv_map_file_;
  }
  if (this != default_instance_) {
  }
}

void ZoneTimeBucket::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ZoneTimeBucket::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ZoneTimeBucket_descriptor_;
}

const ZoneTimeBucket& ZoneTimeBucket::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_entry_2eproto();  return *default_instance_;
}

ZoneTimeBucket* ZoneTimeBucket::default_instance_ = NULL;

ZoneTimeBucket* ZoneTimeBucket::New() const {
  return new ZoneTimeBucket;
}

void ZoneTimeBucket::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (map_name_ != &_default_map_name_) {
        map_name_->clear();
      }
    }
    if (_has_bit(1)) {
      if (classification_ != &_default_classification_) {
        classification_->clear();
      }
    }
    if (_has_bit(2)) {
      if (doctype_ != &_default_doctype_) {
        doctype_->clear();
      }
    }
    if (_has_bit(3)) {
      if (pop_ != &_default_pop_) {
        pop_->clear();
      }
    }
    if (_has_bit(4)) {
      if (psource_ != &_default_psource_) {
        psource_->clear();
      }
    }
    if (_has_bit(5)) {
      if (result_id_ != &_default_result_id_) {
        result_id_->clear();
      }
    }
    if (_has_bit(6)) {
      if (db_key_ != &_default_db_key_) {
        db_key_->clear();
      }
    }
    if (_has_bit(7)) {
      if (db_path_ != &_default_db_path_) {
        db_path_->clear();
      }
    }
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (_has_bit(8)) {
      if (kv_map_file_ != &_default_kv_map_file_) {
        kv_map_file_->clear();
      }
    }
  }
  map_entry_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ZoneTimeBucket::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string map_name = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_map_name()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->map_name().data(), this->map_name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_classification;
        break;
      }
      
      // required string classification = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_classification:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_classification()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->classification().data(), this->classification().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_doctype;
        break;
      }
      
      // required string doctype = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_doctype:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_doctype()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->doctype().data(), this->doctype().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_pop;
        break;
      }
      
      // required string pop = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_pop:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_pop()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->pop().data(), this->pop().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_psource;
        break;
      }
      
      // required string psource = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_psource:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_psource()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->psource().data(), this->psource().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_result_id;
        break;
      }
      
      // required string result_id = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_result_id:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_result_id()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->result_id().data(), this->result_id().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_db_key;
        break;
      }
      
      // required string db_key = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_db_key:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_db_key()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->db_key().data(), this->db_key().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(66)) goto parse_db_path;
        break;
      }
      
      // required string db_path = 8;
      case 8: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_db_path:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_db_path()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->db_path().data(), this->db_path().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(74)) goto parse_kv_map_file;
        break;
      }
      
      // optional string kv_map_file = 9;
      case 9: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_kv_map_file:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_kv_map_file()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->kv_map_file().data(), this->kv_map_file().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(82)) goto parse_map_entry;
        break;
      }
      
      // repeated .cloudflare.ZoneTimeBucket.Counter map_entry = 10;
      case 10: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_map_entry:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_map_entry()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(82)) goto parse_map_entry;
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ZoneTimeBucket::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string map_name = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->map_name().data(), this->map_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->map_name(), output);
  }
  
  // required string classification = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->classification().data(), this->classification().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->classification(), output);
  }
  
  // required string doctype = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->doctype().data(), this->doctype().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->doctype(), output);
  }
  
  // required string pop = 4;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->pop().data(), this->pop().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->pop(), output);
  }
  
  // required string psource = 5;
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->psource().data(), this->psource().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->psource(), output);
  }
  
  // required string result_id = 6;
  if (_has_bit(5)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->result_id().data(), this->result_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->result_id(), output);
  }
  
  // required string db_key = 7;
  if (_has_bit(6)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->db_key().data(), this->db_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      7, this->db_key(), output);
  }
  
  // required string db_path = 8;
  if (_has_bit(7)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->db_path().data(), this->db_path().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      8, this->db_path(), output);
  }
  
  // optional string kv_map_file = 9;
  if (_has_bit(8)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->kv_map_file().data(), this->kv_map_file().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      9, this->kv_map_file(), output);
  }
  
  // repeated .cloudflare.ZoneTimeBucket.Counter map_entry = 10;
  for (int i = 0; i < this->map_entry_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      10, this->map_entry(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ZoneTimeBucket::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string map_name = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->map_name().data(), this->map_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->map_name(), target);
  }
  
  // required string classification = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->classification().data(), this->classification().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->classification(), target);
  }
  
  // required string doctype = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->doctype().data(), this->doctype().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->doctype(), target);
  }
  
  // required string pop = 4;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->pop().data(), this->pop().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        4, this->pop(), target);
  }
  
  // required string psource = 5;
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->psource().data(), this->psource().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->psource(), target);
  }
  
  // required string result_id = 6;
  if (_has_bit(5)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->result_id().data(), this->result_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->result_id(), target);
  }
  
  // required string db_key = 7;
  if (_has_bit(6)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->db_key().data(), this->db_key().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        7, this->db_key(), target);
  }
  
  // required string db_path = 8;
  if (_has_bit(7)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->db_path().data(), this->db_path().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        8, this->db_path(), target);
  }
  
  // optional string kv_map_file = 9;
  if (_has_bit(8)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->kv_map_file().data(), this->kv_map_file().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        9, this->kv_map_file(), target);
  }
  
  // repeated .cloudflare.ZoneTimeBucket.Counter map_entry = 10;
  for (int i = 0; i < this->map_entry_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        10, this->map_entry(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ZoneTimeBucket::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string map_name = 1;
    if (has_map_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->map_name());
    }
    
    // required string classification = 2;
    if (has_classification()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->classification());
    }
    
    // required string doctype = 3;
    if (has_doctype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->doctype());
    }
    
    // required string pop = 4;
    if (has_pop()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->pop());
    }
    
    // required string psource = 5;
    if (has_psource()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->psource());
    }
    
    // required string result_id = 6;
    if (has_result_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->result_id());
    }
    
    // required string db_key = 7;
    if (has_db_key()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->db_key());
    }
    
    // required string db_path = 8;
    if (has_db_path()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->db_path());
    }
    
  }
  if (_has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    // optional string kv_map_file = 9;
    if (has_kv_map_file()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->kv_map_file());
    }
    
  }
  // repeated .cloudflare.ZoneTimeBucket.Counter map_entry = 10;
  total_size += 1 * this->map_entry_size();
  for (int i = 0; i < this->map_entry_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->map_entry(i));
  }
  
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ZoneTimeBucket::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ZoneTimeBucket* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ZoneTimeBucket*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ZoneTimeBucket::MergeFrom(const ZoneTimeBucket& from) {
  GOOGLE_CHECK_NE(&from, this);
  map_entry_.MergeFrom(from.map_entry_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_map_name(from.map_name());
    }
    if (from._has_bit(1)) {
      set_classification(from.classification());
    }
    if (from._has_bit(2)) {
      set_doctype(from.doctype());
    }
    if (from._has_bit(3)) {
      set_pop(from.pop());
    }
    if (from._has_bit(4)) {
      set_psource(from.psource());
    }
    if (from._has_bit(5)) {
      set_result_id(from.result_id());
    }
    if (from._has_bit(6)) {
      set_db_key(from.db_key());
    }
    if (from._has_bit(7)) {
      set_db_path(from.db_path());
    }
  }
  if (from._has_bits_[8 / 32] & (0xffu << (8 % 32))) {
    if (from._has_bit(8)) {
      set_kv_map_file(from.kv_map_file());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ZoneTimeBucket::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ZoneTimeBucket::CopyFrom(const ZoneTimeBucket& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ZoneTimeBucket::IsInitialized() const {
  if ((_has_bits_[0] & 0x000000ff) != 0x000000ff) return false;
  
  for (int i = 0; i < map_entry_size(); i++) {
    if (!this->map_entry(i).IsInitialized()) return false;
  }
  return true;
}

void ZoneTimeBucket::Swap(ZoneTimeBucket* other) {
  if (other != this) {
    std::swap(map_name_, other->map_name_);
    std::swap(classification_, other->classification_);
    std::swap(doctype_, other->doctype_);
    std::swap(pop_, other->pop_);
    std::swap(psource_, other->psource_);
    std::swap(result_id_, other->result_id_);
    std::swap(db_key_, other->db_key_);
    std::swap(db_path_, other->db_path_);
    std::swap(kv_map_file_, other->kv_map_file_);
    map_entry_.Swap(&other->map_entry_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ZoneTimeBucket::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ZoneTimeBucket_descriptor_;
  metadata.reflection = ZoneTimeBucket_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace cloudflare

// @@protoc_insertion_point(global_scope)
