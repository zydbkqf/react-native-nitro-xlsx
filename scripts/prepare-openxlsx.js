import { execSync } from 'child_process';
import { existsSync, mkdirSync, writeFileSync, symlinkSync, readFileSync } from 'fs';
import { dirname, join } from 'path';
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);
const rootDir = dirname(__dirname);

function run(command, options = {}) {
  console.log(`> ${command}`);
  execSync(command, { stdio: 'inherit', ...options });
}

function ensureDir(dir) {
  if (!existsSync(dir)) {
    mkdirSync(dir, { recursive: true });
  }
}

function cloneRepo(url, tag, dest) {
  if (existsSync(dest)) {
    console.log(`Already exists: ${dest}`);
    return;
  }
  run(`git clone --depth 1 --branch ${tag} ${url} ${dest}`);
}

function writeFileIfNotExists(path, content) {
  if (existsSync(path)) {
    console.log(`Already exists: ${path}`);
    return;
  }
  writeFileSync(path, content);
  console.log(`Created: ${path}`);
}

function createSymlinkIfNotExists(target, link) {
  if (existsSync(link)) {
    console.log(`Already exists: ${link}`);
    return;
  }
  symlinkSync(target, link);
  console.log(`Created symlink: ${link} -> ${target}`);
}

function getPackageVersion() {
  const packageJsonPath = join(rootDir, 'package.json');
  const packageJson = JSON.parse(readFileSync(packageJsonPath, 'utf8'));
  return packageJson.version || '0.1.0';
}

const openxlsxDir = join(rootDir, 'OpenXLSX');
const thirdPartyDir = join(openxlsxDir, 'ThirdParty');
const pugixmlDir = join(thirdPartyDir, 'pugixml');
const minizDir = join(thirdPartyDir, 'miniz');

// Clone OpenXLSX and dependencies
cloneRepo('https://github.com/troldal/OpenXLSX.git', 'v0.5.1', openxlsxDir);
ensureDir(thirdPartyDir);
cloneRepo('https://github.com/zeux/pugixml.git', 'v1.16', pugixmlDir);
cloneRepo('https://github.com/richgel999/miniz.git', '3.1.2', minizDir);

// Generate miniz_export.h
const minizExportHeader = `#ifndef MINIZ_EXPORT_H
#define MINIZ_EXPORT_H

#ifndef MINIZ_EXPORT
#define MINIZ_EXPORT
#endif

#ifndef MINIZ_DEPRECATED
#define MINIZ_DEPRECATED
#endif

#ifndef MINIZ_DEPRECATED_EXPORT
#define MINIZ_DEPRECATED_EXPORT MINIZ_EXPORT MINIZ_DEPRECATED
#endif

#ifndef MINIZ_NO_EXPORT
#define MINIZ_NO_EXPORT
#endif

#endif
`;
writeFileIfNotExists(join(minizDir, 'miniz_export.h'), minizExportHeader);

// Generate OpenXLSX-Exports.hpp
const openxlsxExportsHeader = `#ifndef OPENXLSX_EXPORTS_HPP
#define OPENXLSX_EXPORTS_HPP

#ifndef OPENXLSX_EXPORT
#define OPENXLSX_EXPORT
#endif

#ifndef OPENXLSX_HIDDEN
#define OPENXLSX_HIDDEN
#endif

#endif
`;
const headersDir = join(openxlsxDir, 'OpenXLSX', 'headers');
ensureDir(headersDir);
writeFileIfNotExists(join(headersDir, 'OpenXLSX-Exports.hpp'), openxlsxExportsHeader);

// Create symlink for #include <OpenXLSX/XXX.hpp> resolution
createSymlinkIfNotExists('.', join(headersDir, 'OpenXLSX'));

console.log(`OpenXLSX dependencies prepared successfully for react-native-nitro-xlsx v${getPackageVersion()}.`);
