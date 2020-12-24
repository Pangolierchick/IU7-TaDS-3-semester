import random as rnd

LOW_LIMIT = 0
TOP_LIMIT = 10


def fill_file(f, flen):
    for i in range(flen):
        print(rnd.randint(LOW_LIMIT, TOP_LIMIT), file=f)


def main():
    fname = input('Input file name: ')
    flen = int(input("Input file len: "))

    f = open(fname, 'w')

    print("Generating\t\t......\t\t", end="")
    fill_file(f, flen)
    print("Done.")

    f.close()


if __name__ == '__main__':
    main()
