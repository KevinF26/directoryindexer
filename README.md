Indexer is a tool that recursively traverses directories and indexes words found in the corresponding files by alphabetical order and frequency of word occurrence

the index command takes 2 arguments, an output filename for the json output of the indexed words, and the directory or file to be indexed

When run, indexer goes through these steps
1) call traversedir on the second argument
2) traverse dir detects and recursively traverses directory structures and calls index on all regular files
3) index creates entries in a global sorted list of "struct words" which correspond to the current word (as given by
the tokenizer), and each word stores a string corresponding to the word and a list of files with their filenames/frequencies
4) after the global list of words is constructed it is printed out to the file given by argument 1 in json format

traversdir has linear complexity scaling with the number of files/directories it needs to traverse

index has n*m complexity because it must traverse a list of n words and then m files corresponding to the correct word
