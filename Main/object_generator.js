const DIM = 100000;
const fs = require('fs');
const path = '../data/'
const { performance } = require('perf_hooks');

/**
 * Generates a random number within [start, end]
 * @param {int} start 
 * @param {int} end 
 * @returns num
 */
function randomInRange(start, end) {
    return Math.floor(Math.random() * (end - start + 1) + start);
}

/**
 * Generates a random vector of length DIM and writes it to the file that Python will read
 * @param {string} name
 * @returns 
 */
function generateVector(name) {
    // write length random numbers to a file
    let file = path + 'vec_' + name + '.txt';
    let data = '';
    for (let i = 0; i < DIM; i++) {
        data += randomInRange(-100, 100) + '\n';
    }
    fs.writeFileSync(file, data);
    return;
}

let start = performance.now();
generateVector('U');
generateVector('V');
let end = performance.now();

console.log('Time taken: ' + (end - start) + ' [ms]');