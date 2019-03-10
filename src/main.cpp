#include <cmath>
#include <memory>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <fstream>

#include "DS1.h"

#include "lv2.h"

constexpr char URI[] = "https://github.com/LiamLombard/DS1.lv2";
std::unique_ptr<DS1> ds1;
std::ofstream logger;

enum class PortIndex : uint8_t
{
	IN  = 0,
	OUT = 1,
	IN_GAIN = 2,
	OUT_GAIN = 3,
	VOLT_SCALE = 4
};

typedef struct
{
	// Port buffers
	const float* input;
	float*       output;
	const float* inGain;
	const float* outGain;
	const float* voltScale;
} Plugin;

static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                              double rate,
                              const char* bundle_path,
                              const LV2_Feature* const* features)
{
	ds1 = std::make_unique<DS1>((double)1/44100, std::string(getenv("HOME")) + "/.lv2/DS1.lv2/res/nLUT10.csv");
  Plugin* plugin = static_cast<Plugin*>(calloc(1, sizeof(Plugin)));
	return static_cast<LV2_Handle>(plugin);
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data)
{
	Plugin* plugin = static_cast<Plugin*>(instance);

	switch (static_cast<PortIndex>(port))
	{
		case PortIndex::IN:
			plugin->input = static_cast<const float*>(data);
			break;
		case PortIndex::OUT:
			plugin->output = static_cast<float*>(data);
			break;
		case PortIndex::IN_GAIN:
			plugin->inGain = static_cast<const float*>(data);
			break;
		case PortIndex::OUT_GAIN:
			plugin->outGain = static_cast<const float*>(data);
			break;
		case PortIndex::VOLT_SCALE:
			plugin->voltScale = static_cast<const float*>(data);
			break;
		}
}

static void activate(LV2_Handle instance)
{
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
	const Plugin* plugin = static_cast<const Plugin*>(instance);

	const float* const input  = plugin->input;
	float* const output = plugin->output;
	const float IN_GAIN  = *plugin->inGain;
	const float OUT_GAIN  = *plugin->outGain;
	const float VOLTAGE_SCALE  = *plugin->voltScale;


  for (uint32_t pos = 0; pos < n_samples; ++pos)
  {
		const double circuitIn = VOLTAGE_SCALE*IN_GAIN*input[pos];
		const float circuitOut = static_cast<float>(ds1->CalcVLUT(circuitIn));
    output[pos] = OUT_GAIN*circuitOut;
  }
}

static void deactivate(LV2_Handle instance)
{
}

static void cleanup(LV2_Handle instance)
{
  free(instance);
}

static const void* extension_data(const char* uri)
{
	return nullptr;
}

static const LV2_Descriptor descriptor = {
	URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
  {
    case 0:  return &descriptor;
    default: return nullptr;
	}
}