import sys
import json

def main():
    if len(sys.argv) < 3:
        sys.stderr.write(f'Usage: {sys.argv[0]} symbol out_file in_file...\n')
        return 1
    source = ''
    for ifname in sys.argv[3:]:
        with open(ifname, 'r') as ifile:
            source += ifile.read()
            source += '\n'
    source = json.dumps(source)
    symbol = sys.argv[1]
    with open(sys.argv[2], 'w') as ofile:
        ofile.write(f'extern const char {symbol}[];\nconst char {symbol}[] = {source};\n')
    return 0

if __name__ == '__main__':
    sys.exit(main())
