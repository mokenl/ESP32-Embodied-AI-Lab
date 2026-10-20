import numpy as np                                      #as np 是全世界的约定——没有强制要求，但所有 numpy 教程、StackOverflow 答案都这么写。你也这么写，代码才和别人对得上

# 造 30 天的模拟学习时长（分钟）
np.random.seed(42)                                      #np.random.seed(42) —— 设定随机种子。计算机的"随机"其实是伪随机：给定同一个种子，产生的序列完全一样。所以你这台机器上跑出来的 30 个数字，和任何一台机器跑 seed(42) 的结果都相同
minutes = np.random.randint(30, 180, 30)                #np.random.randint(30, 180, 30) —— 三个参数 最小值，最大值（取不到），生成数量；

print("前 7 天：", minutes[:7])                         #[:7] 表示"从开头到下标 6"，不含下标 7
print("总时长  :", minutes.sum(), "分钟")               #sum() 是方法（点在数组后面），不是 np.sum(minutes) 那种函数式写法。两种都行，方法写法更短
print("日均    :", round(minutes.mean(), 1), "分钟")    #minutes.mean() 返回的是 103.63333333333334。round(..., 1) 保留 1 位小数 → 103.6
print("最长一天:", minutes.max(), "分钟")               # minutes.max() 返回的是 179
print("最长那天是第", minutes.argmax() + 1, "天")       # 下标从 0 开始，所以要 +1；argmax() 返回的是下标，不是值

# 统计超过 120 分钟的有几天
hard_days = minutes[minutes > 120]                      #第一步 minutes > 120 —— 把每个元素和 120 比一次，得到一个布尔数组，第二步 minutes[布尔数组] —— 把这个布尔数组当索引用：True 的位置留下，False 的位置丢掉
print("超过 120 分钟的天数：", hard_days.size)           #.size 是元素总数。和 len() 的区别：一维数组两者相同，但二维数组不一样——

# 换算成小时（保留 1 位小数）
hours = np.round(minutes / 60, 1)                       #minutes / 60 —— 数组除以一个数，每个元素都除。这是 numpy 的向量化，不需要写循环；结果自动变成浮点数（即使能整除也是 2.0 而不是 2）；np.round(..., 1) —— numpy 的 round，逐元素保留 1 位小数
print("前 7 天的小时数：", hours[:7])
