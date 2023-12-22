# aflnet-parser

## Build

```bash
git clone --recurse-submodules https://github.com/JeroenRobben/aflnet-parser.git
cmake -S aflnet-parser/ -B aflnet-parser/build
cmake --build aflnet-parser/build --target aflnet-parser -- -j 4
```

## Usage

```bash
aflnet-parser protocol infile outfile
```
### Example

```bash
aflnet-parser FTP input ftp.states
```
