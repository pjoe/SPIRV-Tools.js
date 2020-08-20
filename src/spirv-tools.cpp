#include "spirv-tools/libspirv.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <emscripten/bind.h>
#include <emscripten/val.h>
using namespace emscripten;

auto print_msg_to_stderr = [](spv_message_level_t, const char*,
                              const spv_position_t&, const char* m) {
  std::cerr << "error: " << m << std::endl;
};

std::string dis(std::string const& buffer, uint32_t options) {
  spvtools::SpirvTools core(SPV_ENV_UNIVERSAL_1_3);
  core.SetMessageConsumer(print_msg_to_stderr);

  std::vector<uint32_t> spirv;
  const uint32_t* ptr = reinterpret_cast<const uint32_t*>(buffer.data());
  spirv.assign(ptr, ptr + buffer.size() / 4);
  std::string disassembly;
  if (!core.Disassemble(spirv, &disassembly, options)) return "Error";
  return disassembly;
}

emscripten::val as(std::string const& source) {
  spvtools::SpirvTools core(SPV_ENV_UNIVERSAL_1_3);
  core.SetMessageConsumer(print_msg_to_stderr);

  std::vector<uint32_t> spirv;
  if (!core.Assemble(source, &spirv)) spirv.clear();
  const uint8_t* ptr = reinterpret_cast<const uint8_t*>(spirv.data());
  return emscripten::val(emscripten::typed_memory_view(spirv.size() * 4,
    ptr));
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("dis", &dis);
    function("as", &as);
}