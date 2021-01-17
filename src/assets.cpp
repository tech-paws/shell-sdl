#include "assets.hpp"
#include "platform.hpp"
#include <jpeglib.h>
#include <setjmp.h>

static Result<AssetData> load_shader(RegionMemoryBuffer* dest_memory, const char* asset_name) {
    char path[256] { 0 };
    platform_build_path(&path[0], "assets", "shaders", asset_name);

    FILE* file = fopen(&path[0], "r");

    if (file == nullptr) {
        return result_create_general_error<AssetData>(
            ErrorCode::LoadAsset,
            "Can't open asset: %s", &path[0]
        );
    }

    fseek(file, 0L, SEEK_END);
    u64 size = ftell(file);
    rewind(file);

    // NOTE(sysint64): +1 for terminate symbol \0
    Result<u8*> data_result = region_memory_buffer_alloc(dest_memory, size + 1);

    if (result_has_error(data_result)) {
        return switch_error<AssetData>(data_result);
    }

    u8* data = result_get_payload(data_result);
    size_t offset = 0;
    char c;

    while ((c = getc(file)) != EOF) {
        data[offset] = c;
        offset += 1;
    }

    data[offset] = 0;
    fclose(file);

    AssetData asset_data = {
        .size = size,
        .data = data
    };

    return result_create_success(asset_data);
}

static Result<AssetData> load_png_texture(RegionMemoryBuffer* dest_memory, const char* asset_name) {
    return result_create_general_error<AssetData>(
        ErrorCode::LoadAsset,
        "PNG Not implemented yet"
    );
}

struct JpegErrorMgr {
    jpeg_error_mgr pub;
    jmp_buf set_jmp_buffer;
};

static char jpeg_last_error_msg[JMSG_LENGTH_MAX] {};

METHODDEF(void) jpegErrorExit (j_common_ptr cinfo) {
    JpegErrorMgr* err = (JpegErrorMgr*) cinfo->err;
    (*(cinfo->err->format_message))(cinfo, jpeg_last_error_msg);
    longjmp(err->set_jmp_buffer, 1);
}

static Result<AssetData> load_jpeg_texture(RegionMemoryBuffer* dest_memory, const char* asset_name) {
    char path[256] { 0 };
    platform_build_path(&path[0], "assets", "textures", asset_name);

    FILE* infile = fopen(&path[0], "rb");

    if (infile == nullptr) {
        return result_create_general_error<AssetData>(
            ErrorCode::LoadAsset,
            "Can't open asset: %s", &path[0]
        );
    }

    jpeg_decompress_struct cinfo;
    JpegErrorMgr jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpegErrorExit;

    if (setjmp(jerr.set_jmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

        return result_create_general_error<AssetData>(
            ErrorCode::LoadAsset,
            "JPEG decompress error: %s", &jpeg_last_error_msg[0]
        );
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, 0);

    jpeg_start_decompress(&cinfo);

    const u32 width = cinfo.output_width;
    const u32 height = cinfo.output_height;
    const u32 depth = cinfo.output_components;

    const u64 row_size = width * depth;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_size, 1);

    const u64 texture_size = width * height * depth;
    const u64 size = sizeof(TextureHeader) + texture_size;
    const auto data_result = region_memory_buffer_alloc(dest_memory, size);

    if (result_has_error(data_result)) {
        return switch_error<AssetData>(data_result);
    }

    u8* data = result_get_payload(data_result);

    const TextureHeader texture_header {
        .width = width,
        .height = height,
        .format = TextureFormat::rgb
    };

    memcpy(data, &texture_header, sizeof(TextureHeader));
    u8* image_data = data + sizeof(TextureHeader);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(image_data, buffer[0], row_size);
        image_data += row_size;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    AssetData asset_data = {
        .size = size,
        .data = data
    };

    return result_create_success(asset_data);
}

static Result<AssetData> load_texture(RegionMemoryBuffer* dest_memory, const char* asset_name) {
    const char* dot;
    dot = strrchr(asset_name, '.');

    if (!dot) {
        return result_create_general_error<AssetData>(
            ErrorCode::LoadAsset,
            "Texture don't have extension"
        );
    }

    if (strcmp(dot, ".jpeg") == 0 || strcmp(dot, ".jpg") == 0) {
        return load_jpeg_texture(dest_memory, asset_name);
    }
    else if (strcmp(dot, ".png") == 0) {
        return load_png_texture(dest_memory, asset_name);
    }
    else {
        return result_create_general_error<AssetData>(
            ErrorCode::LoadAsset,
            "Unknown texture extension: %s", dot
        );
    }
}

Result<AssetData> asset_load_data(
    RegionMemoryBuffer* dest_memory,
    const AssetType assetType,
    const char* asset_name
) {
    switch (assetType) {
        case texture:
            return load_texture(dest_memory, asset_name);

        case sfx:
            return result_create_general_error<AssetData>(
                ErrorCode::LoadAsset,
                "Not implemented yet"
            );

        case music:
            return result_create_general_error<AssetData>(
                ErrorCode::LoadAsset,
                "Not implemented yet"
            );

        case shader:
            return load_shader(dest_memory, asset_name);

        case font:
            return result_create_general_error<AssetData>(
                ErrorCode::LoadAsset,
                "Not implemented yet"
            );

        case level:
            return result_create_general_error<AssetData>(
                ErrorCode::LoadAsset,
                "Not implemented yet"
            );

        default:
            return result_create_general_error<AssetData>(
                ErrorCode::LoadAsset,
                "Unknown asset type"
            );
    }
}
