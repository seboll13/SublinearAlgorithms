const DIM = 1000;
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
    // write DIM random numbers to a file
    let file = path + 'vec_' + name + '.txt';
    let data = '';
    for (let i = 0; i < DIM; i++) {
        data += randomInRange(-100, 100) + '\n';
    }
    fs.writeFileSync(file, data);
    return;
}

/**
 * Generates a random matrix of size DIMxDIM and writes it to the file that Python will read
 * @param {string} name 
 * @returns 
 */
function generateMatrix(name) {
    // write DIMxDIM random numbers to a file
    let file = path + 'mat_' + name + '.txt';
    let data = '';
    for (let i = 0; i < DIM; i++) {
        for (let j = 0; j < DIM; j++) {
            data += randomInRange(-100, 100) + ' ';
        }
        data += '\n';
    }
    fs.writeFileSync(file, data);
    return;
}

// // generate vectors
// let start = performance.now();
// generateVector('U');
// generateVector('V');
// let end = performance.now();

// generate matrices
let start = performance.now();
generateMatrix('A');
generateMatrix('B');
let end = performance.now();

console.log('Time taken: ' + (end - start) + ' [ms]');