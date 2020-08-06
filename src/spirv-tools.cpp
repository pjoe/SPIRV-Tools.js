#include <emscripten.h>
#include "spirv-tools/libspirv.hpp"

#include <iostream>
#include <string>
#include <vector>

const std::string source =
    "         OpCapability Linkage "
    "         OpCapability Shader "
    "         OpMemoryModel Logical GLSL450 "
    "         OpSource GLSL 450 "
    "         OpDecorate %spec SpecId 1 "
    "  %int = OpTypeInt 32 1 "
    " %spec = OpSpecConstant %int 0 "
    "%const = OpConstant %int 42";

extern "C" {

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

}