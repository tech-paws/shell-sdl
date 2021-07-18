#pragma once

#include "vm_buffers.hpp"

static const u64 COMMAND_EXECUTE_MACRO = 0x00010001;
static const u64 COMMAND_BEGIN_MACRO = 0x00010002;
static const u64 COMMAND_END_MACRO = 0x00010003;
static const u64 COMMAND_UPDATE_VIEWPORT = 0x00010004;
static const u64 COMMAND_ADD_TEXT_BOUNDARIES = 0x00010005;
static const u64 COMMAND_TOUCH_START = 0x00010006;
static const u64 COMMAND_TOUCH_END = 0x00010007;
static const u64 COMMAND_TOUCH_MOVE = 0x00010008;
static const u64 COMMAND_TOUCH_STATE = 0x00010009;

static const u64 COMMAND_GAPI_DRAW_LINES = 0x00020001;
static const u64 COMMAND_GAPI_DRAW_PATH = 0x00020002;
static const u64 COMMAND_GAPI_DRAW_QUADS = 0x00020003;
static const u64 COMMAND_GAPI_DRAW_CENTERED_QUADS = 0x00020004;
static const u64 COMMAND_GAPI_DRAW_TEXTS = 0x00020005;
static const u64 COMMAND_GAPI_SET_COLOR_PIPELINE = 0x00020006;
static const u64 COMMAND_GAPI_SET_TEXTURE_PIPELINE = 0x00020007;
static const u64 COMMAND_GAPI_SET_VIEWPORT = 0x00020008;

static const u64 COMMAND_TRANSFORM_TRANSLATE = 0x00030001;
static const u64 COMMAND_TRANSFORM_ROTATE = 0x00030002;
static const u64 COMMAND_TRANSFORM_SCALE = 0x00030003;

static const u64 COMMAND_ASSET_LOAD_TEXTURE = 0x00040001;
static const u64 COMMAND_ASSET_LOAD_MACRO = 0x00040002;
static const u64 COMMAND_ASSET_REMOVE_TEXTURE = 0x00040003;
static const u64 COMMAND_ASSET_REMOVE_MACRO = 0x00040004;

static const u64 COMMAND_STATE_UPDATE_VIEW_PORT = 0x00050001;
static const u64 COMMAND_STATE_UPDATE_TOUCH_STATE = 0x00050002;

static const u64 COMMAND_MOUSE_BUTTON_UNKNOWN = 0;
static const u64 COMMAND_MOUSE_BUTTON_LEFT = 1;
static const u64 COMMAND_MOUSE_BUTTON_RIGHT = 2;
static const u64 COMMAND_MOUSE_BUTTON_MIDDLE = 3;

enum Source {
    GAPI = 0,
    Processor = 1,
};

struct BytesBuffer {
    u64 size;
    u8 const* base;
};

struct MutBytesBuffer {
    u64 size;
    u8* base;
};

struct Command {
    u64 id;
    u64 count;
    BytesBuffer from_address;
    BytesBuffer const* payload;
};

struct Commands {
    Command const* data;
    size_t size;
};

extern "C" void tech_paws_vm_init();

extern "C" bool tech_paws_vm_process_commands();

extern "C" void tech_paws_vm_process_render_commands();

extern "C" Commands tech_paws_vm_get_commands();

extern "C" Commands tech_paws_vm_get_gapi_commands();

extern "C" BytesBuffer tech_paws_vm_client_id();

extern "C" void tech_paws_vm_gapi_flush();

extern "C" void tech_paws_vm_process_flush();

extern "C" void tech_paws_vm_flush();

extern "C" void tech_paws_push_command(char const* address, Command command, Source source);

extern "C" void tech_paws_vm_log_trace(char const* message);

extern "C" void tech_paws_vm_log_error(char const* message);

extern "C" void tech_paws_vm_log_warn(char const* message);

extern "C" void tech_paws_vm_log_debug(char const* message);

extern "C" void tech_paws_vm_log_info(char const* message);

extern "C" MutBytesBuffer tech_paws_vm_get_commands_buffer();

extern "C" BytesWriter* tech_paws_begin_command(char const* to, Source source, u64 id);

extern "C" void tech_paws_end_command(char const* to, Source source);
