import express from "express";
import { spawn } from "child_process";

const app = express();
app.use(express.json());

app.post("/run-cpp", (req, res) => {
  const cpp = spawn("./bin_packing");

  let output = "";
  cpp.stdout.on("data", (data) => {
    output += data.toString();
  });

  cpp.stderr.on("data", (data) => {
    console.error("C++ Error:", data.toString());
  });

  cpp.on("close", () => {
    try {
      res.json(JSON.parse(output));
    } catch (err) {
      res.status(500).json({ error: "Invalid JSON from C++" });
    }
  });

  cpp.stdin.write(JSON.stringify(req.body));
  cpp.stdin.end();
});

app.listen(3000, () => {
  console.log("API running on port 3000");
});
