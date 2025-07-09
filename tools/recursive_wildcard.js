const fs = require("fs");
const path = require("path");

// Ensure there are the appropriate command line args
if (process.argv.length < 4) return;

const rootPath = process.argv[2];
const pattern = process.argv[3];
const matchingFiles = [];


// Open the root dir
const rootDir = fs.opendirSync(rootPath);


/**
 * @param {fs.Dir} dirObj 
 */
function processDir(dirObj, dirPath) {
    // Iterate the directory items
    let entry = null
    while ((entry = dirObj.readSync()) !== null) {
        const entryPath = path.join(dirPath, entry.name);

        if (!entry.isFile()) processDir(fs.opendirSync(entryPath), entryPath);
        
        if (entry.name.match(pattern)) matchingFiles.push(entryPath);
    }
}


processDir(rootDir, rootPath);

console.log(matchingFiles.map(e => e.replaceAll("\\", "/")).join(" "));