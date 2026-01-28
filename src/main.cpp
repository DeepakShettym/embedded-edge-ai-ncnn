#include <ncnn/net.h>
#include <stdio.h>

int main() {
    printf("AI Inference Service starting...\n");

    ncnn::Net net;
    net.opt.use_vulkan_compute = false;
    net.opt.num_threads = 4;

    // Load model
    if (net.load_param("/data/local/tmp/squeezenet_v1.1.param") != 0 ||
        net.load_model("/data/local/tmp/squeezenet_v1.1.bin") != 0) {
        printf("❌ Failed to load model\n");
        return -1;
    }

    printf("✅ Model loaded successfully\n");

    // Dummy input
    ncnn::Mat input(227, 227, 3);
    input.fill(0.5f);

    ncnn::Extractor ex = net.create_extractor();
    ex.input("data", input);

    ncnn::Mat output;
    ex.extract("prob", output);

    printf("🎯 Inference done\n");
    printf("Output size: %d\n", output.w);

    return 0;
}
