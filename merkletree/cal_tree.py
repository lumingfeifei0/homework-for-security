#由下面一行计算上面一行的哈希值
def get_row(arr,func):
    row = []
    lenth = len(arr)
    if lenth % 2 == 0:
        for i in range(0,lenth,2):
            row.append(func(arr[i]+arr[i+1]))
    else:
        for i in range(0,lenth,2):
            row.append(func(arr[i]+arr[i+1]))
        row.append(func(arr[lenth-1]))
    return row

#由分散的数据包生成Merkel Tree，结果按树上各结点从下到上从左到右保存到列表中
#根哈希即为最后一个
def get_merkle_tree(arr,func):
    final_arr = []
    if len(arr) == 1:
        final_arr.append(func(arr[0]))
        return final_arr
    
    for i in range(len(arr)):
        final_arr.append(func(arr[i]))
    tem_arr = final_arr

    while len(tem_arr) > 1:
        tem_arr = get_row(tem_arr,func)
        final_arr.extend(tem_arr)
    
    return final_arr