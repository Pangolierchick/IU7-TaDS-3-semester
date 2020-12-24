import random as rnd
from tqdm import tqdm


CARS = ["audi", "ford", "honda", "wolkswagen", "sizuki", "nissan", "toyota"]
COLORS = ["black", "yellow", 'magenta', 'white', 'green', 'blue', 'cyan', 'red']
COUNTRIES = ['USA', 'Germany', 'Russia', 'Japan', 'China', 'Taiwan']

def rnd_yes_no():
    a = rnd.randint(0, 100)
    
    if (a > 50):
        return 'y'
    return 'n'

def gen_struct():
    brand = rnd.choice(CARS)
    country = rnd.choice(COUNTRIES)
    cost = rnd.randint(500, 2000)
    color = rnd.choice(COLORS)
    is_new = rnd_yes_no()

    if (is_new == "y"):
        warranty = rnd.randint(1, 15)

        return (brand, country, cost, color, is_new, warranty)
    else:
        prod_year = rnd.randint(1980, 2015)
        mileage = rnd.randint(100, 1000)
        repair_num = rnd.randint(0, 5)
        owner_num = rnd.randint(0, 5)

        return (brand, country, cost, color, is_new, prod_year, mileage, repair_num, owner_num)
    
def write_struct(struct, file):
    print(struct[0], file=file)
    print(struct[1], file=file)
    print(struct[2], file=file)
    print(struct[3], file=file)
    print(struct[4], file=file)
    print(struct[5], file=file)

    if (struct[4] == 'n'):
        print(struct[6], file=file)
        print(struct[7], file=file)
        print(struct[8], file=file)

def main():
    filename = input("Input filename: ")
    file = open(filename, 'w')
    
    struct_num = int(input("Input struct number: "))

    for i in tqdm(range(struct_num)):
        struct = gen_struct()
        write_struct(struct, file)
        


    file.close()

if __name__ == "__main__":
    main()
