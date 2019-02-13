# -*- coding: utf-8 -*-
import unittest

# 求两个数字的最大公约数（欧几里得算法）
def gcd(a, b):
    while 1:
        if b == 0:
            return a
        a, b = b, a % b

'''
扩展欧几里的算法
计算 ax + by = 1中的x与y的整数解（a与b互质）
'''
def ext_gcd(a, b):
    if b == 0:
        x1 = 1
        y1 = 0
        x = x1
        y = y1
        r = a
        return r, x, y
    else:
        r, x1, y1 = ext_gcd(b, a % b)
        x = y1
        y = x1 - a / b * y1
        return r, x, y


print ext_gcd(11, 13)

exit(0)


'''
超大整数超大次幂然后对超大的整数取模
(base ^ exponent) mod n
'''
def exp_mode(base, exponent, n):
    bin_array = bin(exponent)[2:][::-1]
    r = len(bin_array)
    base_array = []

    pre_base = base
    base_array.append(pre_base)

    for _ in range(r - 1):
        next_base = (pre_base * pre_base) % n
        base_array.append(next_base)
        pre_base = next_base

    a_w_b = __multi(base_array, bin_array)
    return a_w_b % n

def __multi(array, bin_array):
    result = 1
    for index in range(len(array)):
        a = array[index]
        if not int(bin_array[index]):
            continue
        result *= a
    return result

# 生成公钥私钥，p、q为两个超大质数
def gen_key(p, q):
    n = p * q
    fy = (p - 1) * (q - 1)      # 计算与n互质的整数个数 欧拉函数
    e = 3889                    # 选取e   一般选取65537
    # generate d
    a = e
    b = fy
    r, x, y = ext_gcd(a, b)
    d = x
    # 返回：   公钥     私钥
    return (n, e), (n, d)

# 加密 m是被加密的信息 加密成为c
def encrypt(m, pubkey):
    n = pubkey[0]
    e = pubkey[1]

    c = exp_mode(m, e, n)
    return c

# 解密 c是密文，解密为明文m
def decrypt(c, selfkey):
    n = selfkey[0]
    d = selfkey[1]

    m = exp_mode(c, d, n)
    return m


class TestCKL(unittest.TestCase):

    def test_gcd(self):
        self.assertEqual(gcd(65537, 34234247), 1)
        self.assertEqual(gcd(6521332, 34234242), 2)
        self.assertEqual(gcd(6521349, 34234254), 3)
        for i in range(2, 65537):
            self.assertEqual(gcd(65537, i), 1)

def main():
    # unittest.main()

    a = 652547036313776700990791694887580693
    b = 2243609008805117553865
    print gcd(a, b)
    return 0

    '''公钥私钥中用到的两个大质数p,q，都是1024位'''
    p = 106697219132480173106064317148705638676529121742557567770857687729397446898790451577487723991083173010242416863238099716044775658681981821407922722052778958942891831033512463262741053961681512908218003840408526915629689432111480588966800949428079015682624591636010678691927285321708935076221951173426894836169
    q = 144819424465842307806353672547344125290716753535239658417883828941232509622838692761917211806963011168822281666033695157426515864265527046213326145174398018859056439431422867957079149967592078894410082695714160599647180947207504108618794637872261572262805565517756922288320779308895819726074229154002310375209
    '''生成公钥私钥'''
    pubkey, selfkey = gen_key(p, q)
    '''需要被加密的信息转化成数字，长度小于秘钥n的长度，如果信息长度大于n的长度，那么分段进行加密，分段解密即可。'''
    m = 1356205320457610288745198967657644166379972189839804389074591563666634066646564410685955217825048626066190866536592405966964024022236587593447122392540038493893121248948780525117822889230574978651418075403357439692743398250207060920929117606033490559159560987768768324823011579283223392964454439904542675637683985296529882973798752471233683249209762843835985174607047556306705224118165162905676610067022517682197138138621344578050034245933990790845007906416093198845798901781830868021761765904777531676765131379495584915533823288125255520904108500256867069512326595285549579378834222350197662163243932424184772115345
    '''信息加密，m被加密的信息，c是加密后的信息'''
    c = encrypt(m, pubkey)
    print c
    '''信息解密'''
    d = decrypt(c, selfkey)
    print d

if __name__ == '__main__':
    main()