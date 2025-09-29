import { spawn } from 'child_process';

export default async function handler(req, res) {
  if (req.method !== 'POST') {
    res.status(405).send('Only POST allowed');
    return;
  }

  const input = JSON.stringify(req.body);

  const cppProcess = spawn('./bin_packing', [], { stdio: ['pipe', 'pipe', 'inherit'] });
  let output = '';

  cppProcess.stdout.on('data', (data) => {
    output += data.toString();
  });

  cppProcess.on('close', () => {
    res.status(200).json(JSON.parse(output));
  });

  cppProcess.stdin.write(input);
  cppProcess.stdin.end();
}