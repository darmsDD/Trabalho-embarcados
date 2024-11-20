// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from drive_base_msgs:msg/TRVCommand.idl
// generated code does not contain a copyright notice
#include "drive_base_msgs/msg/detail/trv_command__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <cstddef>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "drive_base_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "drive_base_msgs/msg/detail/trv_command__struct.h"
#include "drive_base_msgs/msg/detail/trv_command__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif

#include "drive_base_msgs/msg/detail/command_header__functions.h"  // header

// forward declare type support functions

bool cdr_serialize_drive_base_msgs__msg__CommandHeader(
  const drive_base_msgs__msg__CommandHeader * ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool cdr_deserialize_drive_base_msgs__msg__CommandHeader(
  eprosima::fastcdr::Cdr & cdr,
  drive_base_msgs__msg__CommandHeader * ros_message);

size_t get_serialized_size_drive_base_msgs__msg__CommandHeader(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_drive_base_msgs__msg__CommandHeader(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

bool cdr_serialize_key_drive_base_msgs__msg__CommandHeader(
  const drive_base_msgs__msg__CommandHeader * ros_message,
  eprosima::fastcdr::Cdr & cdr);

size_t get_serialized_size_key_drive_base_msgs__msg__CommandHeader(
  const void * untyped_ros_message,
  size_t current_alignment);

size_t max_serialized_size_key_drive_base_msgs__msg__CommandHeader(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, drive_base_msgs, msg, CommandHeader)();


using _TRVCommand__ros_msg_type = drive_base_msgs__msg__TRVCommand;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_drive_base_msgs
bool cdr_serialize_drive_base_msgs__msg__TRVCommand(
  const drive_base_msgs__msg__TRVCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: header
  {
    cdr_serialize_drive_base_msgs__msg__CommandHeader(
      &ros_message->header, cdr);
  }

  // Field name: translational_velocity
  {
    cdr << ros_message->translational_velocity;
  }

  // Field name: rotational_velocity
  {
    cdr << ros_message->rotational_velocity;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_drive_base_msgs
bool cdr_deserialize_drive_base_msgs__msg__TRVCommand(
  eprosima::fastcdr::Cdr & cdr,
  drive_base_msgs__msg__TRVCommand * ros_message)
{
  // Field name: header
  {
    cdr_deserialize_drive_base_msgs__msg__CommandHeader(cdr, &ros_message->header);
  }

  // Field name: translational_velocity
  {
    cdr >> ros_message->translational_velocity;
  }

  // Field name: rotational_velocity
  {
    cdr >> ros_message->rotational_velocity;
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_drive_base_msgs
size_t get_serialized_size_drive_base_msgs__msg__TRVCommand(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _TRVCommand__ros_msg_type * ros_message = static_cast<const _TRVCommand__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: header
  current_alignment += get_serialized_size_drive_base_msgs__msg__CommandHeader(
    &(ros_message->header), current_alignment);

  // Field name: translational_velocity
  {
    size_t item_size = sizeof(ros_message->translational_velocity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: rotational_velocity
  {
    size_t item_size = sizeof(ros_message->rotational_velocity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_drive_base_msgs
size_t max_serialized_size_drive_base_msgs__msg__TRVCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: header
  {
    size_t array_size = 1;
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_drive_base_msgs__msg__CommandHeader(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: translational_velocity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: rotational_velocity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = drive_base_msgs__msg__TRVCommand;
    is_plain =
      (
      offsetof(DataType, rotational_velocity) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_drive_base_msgs
bool cdr_serialize_key_drive_base_msgs__msg__TRVCommand(
  const drive_base_msgs__msg__TRVCommand * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: header
  {
    cdr_serialize_key_drive_base_msgs__msg__CommandHeader(
      &ros_message->header, cdr);
  }

  // Field name: translational_velocity
  {
    cdr << ros_message->translational_velocity;
  }

  // Field name: rotational_velocity
  {
    cdr << ros_message->rotational_velocity;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_drive_base_msgs
size_t get_serialized_size_key_drive_base_msgs__msg__TRVCommand(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _TRVCommand__ros_msg_type * ros_message = static_cast<const _TRVCommand__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: header
  current_alignment += get_serialized_size_key_drive_base_msgs__msg__CommandHeader(
    &(ros_message->header), current_alignment);

  // Field name: translational_velocity
  {
    size_t item_size = sizeof(ros_message->translational_velocity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: rotational_velocity
  {
    size_t item_size = sizeof(ros_message->rotational_velocity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_drive_base_msgs
size_t max_serialized_size_key_drive_base_msgs__msg__TRVCommand(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: header
  {
    size_t array_size = 1;
    last_member_size = 0;
    for (size_t index = 0; index < array_size; ++index) {
      bool inner_full_bounded;
      bool inner_is_plain;
      size_t inner_size;
      inner_size =
        max_serialized_size_key_drive_base_msgs__msg__CommandHeader(
        inner_full_bounded, inner_is_plain, current_alignment);
      last_member_size += inner_size;
      current_alignment += inner_size;
      full_bounded &= inner_full_bounded;
      is_plain &= inner_is_plain;
    }
  }

  // Field name: translational_velocity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: rotational_velocity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = drive_base_msgs__msg__TRVCommand;
    is_plain =
      (
      offsetof(DataType, rotational_velocity) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _TRVCommand__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const drive_base_msgs__msg__TRVCommand * ros_message = static_cast<const drive_base_msgs__msg__TRVCommand *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_drive_base_msgs__msg__TRVCommand(ros_message, cdr);
}

static bool _TRVCommand__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  drive_base_msgs__msg__TRVCommand * ros_message = static_cast<drive_base_msgs__msg__TRVCommand *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_drive_base_msgs__msg__TRVCommand(cdr, ros_message);
}

static uint32_t _TRVCommand__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_drive_base_msgs__msg__TRVCommand(
      untyped_ros_message, 0));
}

static size_t _TRVCommand__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_drive_base_msgs__msg__TRVCommand(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_TRVCommand = {
  "drive_base_msgs::msg",
  "TRVCommand",
  _TRVCommand__cdr_serialize,
  _TRVCommand__cdr_deserialize,
  _TRVCommand__get_serialized_size,
  _TRVCommand__max_serialized_size,
  nullptr
};

static rosidl_message_type_support_t _TRVCommand__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_TRVCommand,
  get_message_typesupport_handle_function,
  &drive_base_msgs__msg__TRVCommand__get_type_hash,
  &drive_base_msgs__msg__TRVCommand__get_type_description,
  &drive_base_msgs__msg__TRVCommand__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, drive_base_msgs, msg, TRVCommand)() {
  return &_TRVCommand__type_support;
}

#if defined(__cplusplus)
}
#endif
