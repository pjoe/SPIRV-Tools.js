#include <emscripten.h>
#include "spirv-tools/libspirv.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <emscripten/bind.h>
using namespace emscripten;


const std::string source =
    "         OpCapability Linkage "
    "         OpCapability Shader "
    "         OpMemoryModel Logical GLSL450 "
    "         OpSource GLSL 450 "
    "         OpDecorate %spec SpecId 1 "
    "  %int = OpTypeInt 32 1 "
    " %spec = OpSpecConstant %int 0 "
    "%const = OpConstant %int 42";

EMSCRIPTEN_KEEPALIVE
int foo() {
  spvtools::SpirvTools core(SPV_ENV_UNIVERSAL_1_3);
  auto print_msg_to_stderr = [](spv_message_level_t, const char*,
                                const spv_position_t&, const char* m) {
    std::cerr << "error: " << m << std::endl;
  };
  core.SetMessageConsumer(print_msg_to_stderr);

  std::vector<uint32_t> spirv;
  if (!core.Assemble(source, &spirv)) return 1;

  std::string disassembly;
  if (!core.Disassemble(spirv, &disassembly)) return 1;
  std::cout << disassembly << "\n";
  return 0;
}

std::string dis(std::string const& buffer, uint32_t options) {
  spvtools::SpirvTools core(SPV_ENV_UNIVERSAL_1_3);
  auto print_msg_to_stderr = [](spv_message_level_t, const char*,
                                const spv_position_t&, const char* m) {
    std::cerr << "error: " << m << std::endl;
  };
  core.SetMessageConsumer(print_msg_to_stderr);

  std::vector<uint32_t> spirv;
  const uint32_t* ptr = reinterpret_cast<const uint32_t*>(buffer.data());
  spirv.assign(ptr, ptr + buffer.size() / 4);
  std::string disassembly;
  if (!core.Disassemble(spirv, &disassembly, options)) return "Error";
  return disassembly;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("foo", &foo);
    function("dis", &dis);
}