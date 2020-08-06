const spirvTools = require("../dist/web/spirv-tools");

const test = async () => {
  const spirvTest = await new Promise((resolve) => {
    spirvTools().then((m) => {
      resolve(m.cwrap("foo", "void"));
    });
  });
  spirvTest();
};

test();
