const spirvTools = require("../dist/web/spirv-tools");
const fs = require("fs");
const util = require("util");
const readFile = util.promisify(fs.readFile);

const SPV_PATH = "./SPIRV-Tools/test/fuzzers/corpora/spv/simple.spv";

const test = async () => {
  const buffer = await readFile(SPV_PATH);
  const spv = await spirvTools();
  const result = spv.dis(
    buffer,
    spv.SPV_BINARY_TO_TEXT_OPTION_INDENT |
      spv.SPV_BINARY_TO_TEXT_OPTION_FRIENDLY_NAMES |
      spv.SPV_BINARY_TO_TEXT_OPTION_COLOR
  );
  console.log("result:\n", result);
};

test();
