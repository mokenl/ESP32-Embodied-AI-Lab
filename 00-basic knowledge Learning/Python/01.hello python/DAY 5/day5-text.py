import task_manager as tm   #import —— 关键字。告诉 Python："我要去加载一个模块。
                            #task_manager —— 模块名。Python 会按名字去找文件，找到 task_manager.py 就加载它。
                            # ​注意这里写的是文件名去掉 .py，不能写 task_manager.py
                            #as tm —— 给这个模块起个别名。以后在代码里写 tm 就等于写 task_manager。as 是"当作"的意思
tasks = []
tm.add_task(tasks, "学函数与模块", 120, ("第1周", "Python基础"))
tm.add_task(tasks, "重构学习计划管理器", 60, ("第1周", "Python基础"))

tm.show_all(tasks)
print("总投入：", tm.total_minutes(tasks), "分钟")
