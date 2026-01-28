#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <ncnn/net.h>
#include <ncnn/mat.h>
#include <stdio.h>

int main() {
    printf("AI Image Inference starting...\n");

    // -----------------------------
    // Load image from file
    // -----------------------------
    int img_w, img_h, img_c;
    unsigned char* img = stbi_load(
        "/data/local/tmp/test.jpg",
        &img_w,
        &img_h,
        &img_c,
        3
    );

    if (!img) {
        printf("❌ Failed to load image\n");
        return -1;
    }

    printf("✅ Image loaded: %dx%d\n", img_w, img_h);

    // -----------------------------
    // Convert image to ncnn::Mat
    // -----------------------------
    ncnn::Mat input = ncnn::Mat::from_pixels_resize(
        img,
        ncnn::Mat::PIXEL_RGB,
        img_w,
        img_h,
        227,
        227
    );

    stbi_image_free(img);

    // Mean normalization (SqueezeNet)
    const float mean_vals[3] = {104.f, 117.f, 123.f};
    input.substract_mean_normalize(mean_vals, nullptr);

    // -----------------------------
    // Load model
    // -----------------------------
    ncnn::Net net;
    net.opt.use_vulkan_compute = false;
    net.opt.num_threads = 4;

    if (net.load_param("/data/local/tmp/squeezenet_v1.1.param") != 0 ||
        net.load_model("/data/local/tmp/squeezenet_v1.1.bin") != 0) {
        printf("❌ Failed to load model\n");
        return -1;
    }

    printf("✅ Model loaded successfully\n");

    // -----------------------------
    // Run inference
    // -----------------------------
    ncnn::Extractor ex = net.create_extractor();
    ex.input("data", input);

    ncnn::Mat output;
    ex.extract("prob", output);

    // -----------------------------
    // Find top-1 result
    // -----------------------------
    int best_class = 0;
    float best_score = output[0];

    for (int i = 1; i < output.w; i++) {
        if (output[i] > best_score) {
            best_score = output[i];
            best_class = i;
        }
    }

    printf("🎯 Prediction done\n");
    printf("Top class index: %d\n", best_class);
    printf("Confidence: %f\n", best_score);

    return 0;
}
