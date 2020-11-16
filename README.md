# Multiprocess programming

This is an OS course project, for implementing multiprocess programming. In this project you give program some assets about shop data in 
city of provinces, app will tell you the max or min in the period you specified.

## How it works?

First process is `FindMaxOrMin`. This process fork provinces in assets folder. After `Province` forks 
cities. After that `City` forks shops. `Shop` find minimum or maximum in specified shop and return anser to 
`City`. `City` will find answer between shops and return to `Province`. `Province` do this as well and return 
to `FindMaxOrMin` and it will print answers.

The messages are send between processes using Pipes(NamedPipes for child to parent and UnnamedPipes for parent to child).

## Inputs 

First you shuld put your assets in side of makefile. Also commands mus be there.

Commands look like:

```
MAX/MIN productId startDate endDate
```

Also data file are in `.csv` in below format:

```
date,productId,price
```

## How to run?

To run the program firs `make` using makefile given to compile using g++:

```
make
```

Then go to `bin` folder, then run:

```
./FindMaxOrMin commandsFileName assetsDirName
```

To clean and undo compilation use:

```
make clean
```

*Made By Amirhossein Abaskohi*