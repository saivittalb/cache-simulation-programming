# Cache Simulation Programming

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![PR's Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat)](http://makeapullrequest.com)
[![GitHub followers](https://img.shields.io/github/followers/saivittalb.svg?style=social&label=Follow)](https://github.com/saivittalb?tab=followers)
[![Twitter Follow](https://img.shields.io/twitter/follow/saivittalb.svg?style=social)](https://twitter.com/saivittalb)

Implementation of single-level cache simulator and analyze the performance of various cache architectures using real-world program traces. Two models were developed; one with prefetching and without prefetching.

Developed as a part of an assignment for the course EE 318 – Computer Architecture and Design.

Refer to the further design specifications in ```doc``` folder. This project requires GCC compiler.

###### Note

Developed with C++ 4.2.1 (GCC Apple clang version 12.0.5 (clang-1205.0.22.9)).
Editor used was Visual Studio Code 1.56.2.

## Table of contents

* [License](#license)
* [Setup](#setup)
  * [Traces](#traces)
  * [Without prefetch](#without-prefetch)
    * [Compilation for no prefetch cache](#compilation-for-no-prefetch-cache)
    * [Usage for no prefetch cache](#usage-for-no-prefetch-cache)
  * [With prefetch](#with-prefetch)
    * [Compilation for prefetch cache](#compilation-for-prefetch-cache)
    * [Usage for prefetch cache](#usage-for-prefetch-cache)
* [Results](#results)
  * [For no prefetch](#for-no-prefetch)
  * [For prefetch](#for-prefetch)
* [Contributing](#contributing)

## License

This project is licensed under the MIT License, a short and simple permissive license with conditions only requiring preservation of copyright and license notices. Licensed works, modifications and larger works may be distributed under different terms and without source code.

<p align="center"> Copyright (c) 2021 Sai Vittal B. All rights reserved.</p>

## Setup

### Traces

Traces used are available via a compressed file named ```traces.zip```. Download this repo and unzip the file to get access to four unique traces.

Traces should be provided in the following format:

```trace
name
0x<hexadecimal-value>
0x<hexadecimal-value>
...
```

Example:

```trace
adpcm.trace
0x00007FFFCE4F6390
0x00007FFFCE4F628C
0x00007FFFCE4F6290
...
```

### Without prefetch

Implementation is done for the following configuration parameters along with pruning of impractical configurations.

* CSIZE (Cache Size): Total size of the cache. Should be a power of 2.
* BSIZE (Block Size): Cache block size. Should be a power of 2.
* ASSOC (Assciativity): Direct mapped, n-way. 'n' value should be a power of 2.
* RPOL (Replacement Policy): LRU, LFU, CLOCK, RANDOM (can be set only for n-way associative caches).
* WPOL (Write Policy): Write Through, Write Allocate.

#### Compilation for no prefetch cache

* Run the following command to compile in the project directory.

```bash
$ make
```

* Once this step is successful, ```bin``` and ```build``` directories will be generated in the project directory.

* In order to make changes and rerun the program, the ```$ make``` command will still work. But, it is advisable to clean using ```$ make clean``` and then use the ```$ make``` command.

#### Usage for no prefetch cache

* As soon as the compilation is done, run either of the following command in the project directory to get help.

```bash
$ ./bin/Simulator -h

    (or)

$ ./bin/Simulator --help
```

* It is then self explanatory from the output of the above command. The general schema in the project directory is

```bash
$ ./bin/Simulator <options> <input-file-path-here>
```  

* For more comprehendibility, the simulator can be run with two versions of the options. One with shortcut options and the other by specifying full words.

```bash
$ ./bin/Simulator -s 256 -dm 8 -clk -wt traces/adpcm.trace
    
    (or)

$ ./bin/Simulator --size 256 --associativity 4 -least-frequently-used --write-allocate traces/adpcm.trace
```

* The command ```--size``` or ```-s``` take the input in kilobyte and ```--size-b``` or ```-sb``` takes the input in byte. For block size specification, the default input is in bytes. These will also be showcased when the above discussed help command is triggered.

### With prefetch

Implementation is done for the following configuration parameters along with pruning of impractical configurations.

* CSIZE (Cache Size): Total size of the cache. Should be a power of 2.
* BSIZE (Block Size): Cache block size. Should be a power of 2.
* ASSOC (Assciativity): Direct mapped, fully associative, n-way. 'n' value should be a power of 2.
* RPOL (Replacement Policy): LRU (can be set only for associative caches).

Here, if the prefeteched block is in already in the cache, it will not issue a memory read. It is activated only when there is a miss and is deactivated when there is a hit.

#### Compilation for prefetch cache

* Run the following commands to compile in the project directory.

```bash
$ cd prefetch-cache

$ make
```

* Once this step is successful, ```bin``` and ```build``` directories will be generated in the ```prefetch-cache``` directory.

* In order to make changes and rerun the program, the ```$ make``` command will still work. But, it is advisable to clean using ```$ make clean``` and then use the ```$ make``` command.

#### Usage for prefetch cache

* Unlike the other cache simulator, this simulator doesn't have a help command. But the usage is almost similar. The general schema in the ```prefetch-cache``` directory is

```bash
$ ./bin/Simulator <options> <input-file-path-here>
```

* ```direct``` option simulates a direct mapped cache, ```assoc```option simulates a fully associative cache, and ```assoc:n``` option simulates an n-way associative cache.

* Refer below for example usage. This simulates a 2-way associative cache with cache size 32 bytes and block size 4 bytes.

```bash
$ ./bin/Simulator 32 assoc:2 lru 4 ../traces/adpcm.trace
```

* The below example simulates a direct-mapped cache with cache size 64 bytes and block size 8 bytes. Replace ```direct``` with ```assoc``` to simulate a fully associative cache.

```bash
$ ./bin/Simulator 64 direct lru 8 ../traces/adpcm.trace

$ ./bin/Simulator 64 assoc lru 8 ../traces/adpcm.trace
```

## Results

Results report can be studied in ```Report.docx``` document. Exported data as per design specifications can be found in ```exported-data``` directory. Miss rate plots can be viewed in ```Report.docx``` or in the seperate folder named ```plots```.

### For no prefetch

Results report along with miss rate plots are included in a single document named ```Report.docx```.
Exported data is included in an excel file named ```no-prefetch.xlsx``` which has different worksheets for each trace. Miss rate plots can also be viewed in a folder named ```no-prefetch``` in ```plots``` directory.

### For prefetch

Results report along with miss rate plots are included in a single document named ```Report.docx```.
Exported data is included in an excel file named ```prefetch.xlsx``` which has different worksheets for each trace. Miss rate plots can also be viewed in a folder named ```prefetch``` in ```plots``` directory.

## Contributing

* Fork this project by clicking the ```Fork``` button on top right corner of this page.
* Open terminal/console window.
* Clone the repository by running following command in git:

```bash
$ git clone https://github.com/[YOUR-USERNAME]/cache-simulation-programming.git
```

* Add all changes by running this command.

```bash
$ git add .
```

* Or to add specific files only, run this command.

```bash
$ git add path/to/your/file
```

* Commit changes by running these commands.

```bash
$ git commit -m "DESCRIBE YOUR CHANGES HERE"

$ git push origin
```

* Create a Pull Request by clicking the ```New pull request``` button on your repository page.

[![ForTheBadge built-with-love](http://ForTheBadge.com/images/badges/built-with-love.svg)](https://GitHub.com/saivittalb/)
[![ForTheBadge powered-by-electricity](http://ForTheBadge.com/images/badges/powered-by-electricity.svg)](http://ForTheBadge.com)

<p align="center"> Copyright (c) 2021 Sai Vittal B. All rights reserved.</p>
<p align="center"> Made with ❤ by <a href="https://github.com/saivittalb">Sai Vittal B</a></p>
