import tqdm
import time
import os
from typing import List


class SolverBF:
    def __init__(self, ids: List[int]):
        self.id_to_value = {value: i for i, value in enumerate(ids)}
        self.items = [0 for _ in ids]

    def update(self, id0, id1, x):
        i = self.id_to_value[id0]
        i_final = self.id_to_value[id1]
        while i <= i_final:
            self.items[i] += x
            i += 1
            x += 1

    def query(self, id0, id1):
        i0 = self.id_to_value[id0]
        i1 = self.id_to_value[id1]
        max_diff = float("-inf")
        for i in range(i0, i1):
            diff = abs(self.items[i] - self.items[i + 1])
            max_diff = max(max_diff, diff)
        return max_diff


def load_file(file):
    with open(file) as f:
        return [line.strip() for line in f.readlines()]


def solve_bf(test_case):
    print(f"Solving {test_case}")
    emso_file = f"{test_case}.vpisna"
    in_file = f"{test_case}.in"
    out_file = f"{test_case}.out"
    ids = [int(x) for x in load_file(emso_file)]
    prompts = load_file(in_file)
    check_data(ids, prompts, test_case)
    solver = SolverBF(ids)
    answers = []
    t1 = time.time()
    for prompt in tqdm.tqdm(prompts):
        if prompt.startswith("o"):
            _, id0, id1 = prompt.split(" ")
            answers.append(solver.query(int(id0), int(id1)))
        elif prompt.startswith("p"):
            _, id0, id1, x = prompt.split(" ")
            solver.update(int(id0), int(id1), int(x))
        else:
            raise ValueError(":)")
    t2 = time.time()
    print(f"Solved in {t2 - t1:.2f} seconds")
    print("")
    with open(out_file, "w") as f:
        for a in answers:
            print(a, file=f)


def solve_all():
    test_cases = [naloga[:-3] for naloga in os.listdir(".") if naloga.endswith(".in")]
    for test_case in test_cases:
        solve_bf(test_case)


def check_data(ids, prompts, problem_id):
    if ids != sorted(ids):
        raise ValueError("Sort IDS!")
    for id_value in ids:
        if len(str(id_value)) != len(str(ids[0])):
            raise ValueError("Not all ids have the same length")
    for prompt in prompts:
        if prompt.startswith("o"):
            _, e0, e1 = prompt.strip().split(" ")
            if e0 >= e1:
                raise ValueError(f"e0 >= e1: {prompt}")
    if problem_id.startswith("nalogaA"):
        found_query = False
        for prompt in prompts:
            if prompt.startswith("o"):
                found_query = True
            elif prompt.startswith("p") and found_query:
                raise ValueError("p after o in A")
    elif problem_id.startswith("nalogaB"):
        # nothing to do here
        pass
    else:
        raise ValueError(f"Wrong problem: {problem_id}")
    print(f"Checks for {problem_id} passed")


def check_ranges():
    for solution_f in os.listdir("."):
        if solution_f.endswith(".out"):
            with open(solution_f) as f:
                answers = list(map(int, f.readlines()))
            m = max(answers)
            print(f"max({solution_f}) = {m}")


if __name__ == '__main__':
    solve_all()
    # check_ranges()
