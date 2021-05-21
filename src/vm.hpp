#pragma once

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

extern "C" void tech_paws_vm_process_commands();

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
