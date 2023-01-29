// This will generate host-code from pntr header

import { readFile } from 'fs/promises'

const regex = /PNTR_API (.+) (.+)\((.+)\);/gm;
const str = await readFile('build/_deps/pntr-src/pntr.h', 'utf8')

const methods = []
let m
while ((m = regex.exec(str)) !== null) {
  methods.push({
    name: m[2].trim(),
    returns: m[1].trim(),
    args: m[3].split(',').map(s => {
      const i = s.trim().split(' ')
      return [i.at(-1), i.slice(0, -1).join(' ')]
    })
  })
}

console.log(methods)
