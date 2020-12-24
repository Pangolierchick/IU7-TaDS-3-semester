def read_file(f):
    seq = []
    for i in f.readlines():
        seq.append(int(i))
    
    return seq

def find_subseq(seq):
    sub_seq = []

    left = 0
    right = 0

    for i in range(len(seq)):
        if seq[i - 1] <= seq[i]:
            if right - left > 1:
                sub_seq.append(seq[left:right])
            
            left = i
        right += 1
    
    if right - left > 1 and seq[right - 2] > seq[right - 1]:
        sub_seq.append(seq[left:right])
    
    return sub_seq

def invert_sub_seq(sub_seq):
    for i in range(len(sub_seq)):
        sub_seq[i] = sub_seq[i][::-1]
    
def task(file):
    seq = read_file(file)
    sub_seq = find_subseq(seq)
    invert_sub_seq(sub_seq)

    return sub_seq

def main():
    fname = input('Input filename: ')

    try:
        file = open(fname, 'r')
    except:
        print("Unexisting file")

    sub_seq = task(file)

    print(sub_seq)

    file.close()




if __name__ == '__main__':
    main()
