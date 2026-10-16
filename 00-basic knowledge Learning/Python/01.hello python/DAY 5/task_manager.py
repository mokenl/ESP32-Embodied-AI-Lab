def add_task(tasks, name, minutes, stage):
    tasks.append({
        "name": name,
        "minutes": minutes,
        "stage": stage,
        "done": False,
    })

def show_all(tasks):
    if not tasks:
        print("（暂无任务）")
        return
    for i, t in enumerate(tasks, 1):
        mark = "已完成" if t.get("done", False) else "待完成"   #值A if 条件 else 值B.如果条件成立就用值A，否则用值B 
                                                        #有 done 就取它，没有就当作 False。这样缺字段的任务默认显示"未完成"，程序不会崩
        stage = t.get("stage", ("未知", ""))[0]
        print(f"{i}. [{mark}] {t['name']}  {t['minutes']}分钟  {stage}")

def total_minutes(tasks):
    return sum(t["minutes"] for t in tasks)
