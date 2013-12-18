#!/usr/bin/env python
import argparse
import re
import sh
import sys

def cleanup_path(f):
    src_index = f.find('/src/')
    if src_index != -1:
        return f[src_index + 5:]
    return f

class FileInfo(object):
    def __init__(self, filename):
        self.filename = filename
        self.size = 0
        self.symbols = []

    def add_entry(self, name, line_number, size):
        self.size += size
        self.symbols.append((name, line_number, size))

    def pprint(self, verbose):
        print '  %s: size %u' % (cleanup_path(self.filename), self.size)
        if verbose:
            l = sorted(self.symbols, key=lambda x: -x[2])
            for s in l:
                print '    %-36s %6u' % (s[0], s[2])

class SectionInfo(object):
    def __init__(self, name):
        self.name = name
        self.count = 0
        self.size = 0
        self.files = {}

    def add_entry(self, name, filename_line, size):
        self.count += 1
        self.size += size

        cols = filename_line.split(':')
        filename = cols[0]
        line_number = int(cols[1])

        if filename not in self.files:
            self.files[filename] = FileInfo(filename)

        self.files[filename].add_entry(name, line_number, size)

    def pprint(self, summary, verbose):
        print '%s: count %u size %u' % (self.name, self.count, self.size)

        if not summary:
            l = self.files.values()
            l = sorted(l, key=lambda f: -f.size)
            for f in l:
                f.pprint(verbose)


def analyze_sizes(elf_file):
    """ generate a list of valid .bss, .data and .text sizes/counts"""
    infile = sh.arm_none_eabi_nm('-l', '-S', elf_file)

    line_pattern = re.compile(r"""([0-9a-f]+)\s+ # address
                                  ([0-9a-f]+)\s+ # size
                                  ([dDbBtT])\s+ # section type
                                  (\S+) # name
                                  \s*(\S+)?$ # filename + line
                                  """, flags=re.VERBOSE)

    sections = { 'b' : SectionInfo('.bss'),
                 'd' : SectionInfo('.data'),
                 't' : SectionInfo('.text') }


    for line in infile:
        match = line_pattern.match(line)

        if match is None:
            continue

        addr = int(match.group(1), 16)
        size = int(match.group(2), 16)
        section = match.group(3).lower()
        name = match.group(4)
        filename_line = match.group(5)

        if filename_line is None:
            filename_line = 'Unknown:0'

        sections[section].add_entry(name, filename_line, size)

    return sections


if (__name__ == '__main__'):
    parser = argparse.ArgumentParser()
    parser.add_argument('elf_file')
    parser.add_argument('--verbose', action='store_true')
    parser.add_argument('--summary', action='store_true')
    args = parser.parse_args()

    sections = analyze_sizes(args.elf_file)

    sections['b'].pprint(args.summary, args.verbose)
    sections['d'].pprint(args.summary, args.verbose)
    sections['t'].pprint(args.summary, args.verbose)
