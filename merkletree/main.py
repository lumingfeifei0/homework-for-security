import hashlib
from cal_tree import get_merkle_tree

def get_md5(value):
    md = hashlib.md5()
    md.update(value.encode('utf-8'))
    return md.hexdigest()

if __name__ == '__main__':
    initial_arr = input('初始化列表(元素用逗号隔开):').split(",") 
    tree = get_merkle_tree(initial_arr,get_md5)
    print("使用md5做出的merker tree为",tree)
    print("根哈希为{}".format(tree[len(tree)-1]))
    
