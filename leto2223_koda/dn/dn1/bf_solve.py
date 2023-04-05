import tqdm
import bisect
import time
import os


class SolverSemiIntelligent:
    """
    Assumes that query operates on the current data.
    """
    def __init__(self):
        self.items = []
        self.times = []
        self.last_time = -1

    def insert(self, t, x):
        self.items.append(x)
        self.items.sort()  # :)
        self.last_time = max(self.last_time, t)

    def query(self, t, q):
        assert t >= self.last_time >= 0
        i0 = bisect.bisect_left(self.items, q)
        left = i0 - 1
        right = i0
        qualities = []
        n = len(self.items)
        for _ in range(4):
            if 0 <= left and right < n:
                q_left = abs(q - self.items[left])
                q_rigth = abs(q - self.items[right])
                if q_left <= q_rigth:
                    qualities.append(q_left)
                    left -= 1
                else:
                    qualities.append(q_rigth)
                    right += 1
            elif 0 <= left:
                qualities.append(abs(q - self.items[left]))
                left -= 1
            elif right < n:
                qualities.append(abs(q - self.items[right]))
                right += 1
            else:
                print(f"Warning: {self.items}, t = {t}, q = {q} ==> qs = {qualities}")
                break
        assert sorted(qualities) == qualities
        return sum(qualities)


class SolverBF:
    def __init__(self):
        self.items = []

    def insert(self, t, x):
        self.items.append((t, x))

    def query(self, t, q):
        filtered = [abs(x - q) for t1, x in self.items if t1 <= t]
        filtered.sort()
        if len(filtered) < 4:
            print(f"Warning: {self.items}, t = {t}, q = {q} ==> qs = {filtered}")
        return sum(filtered[:4])


def load_file(file, is_input):
    parsed = []
    with open(file) as f:
        for line in f:
            if is_input:
                kind, a, b = line.strip().split(" ")
                parsed.append((kind, int(a), int(b)))
            else:
                parsed.append(int(line.strip()))
    return parsed


def solve_easy(test_case):
    print(f"Solving {test_case}")
    in_file = f"{test_case}.in"
    out_file = f"{test_case}.out"
    prompts = list(enumerate(load_file(in_file, True)))

    query_to_index = {}
    for i, (kind, _, _) in prompts:
        if kind == "p":
            query_to_index[i] = len(query_to_index)

    n_queries = len(query_to_index)
    answers = [-1 for _ in range(n_queries)]
    check = os.path.exists(out_file)
    if check:
        correct_answers = load_file(out_file, False)
    else:
        print("Won't be checking. Will write the answers")
        correct_answers = [-1 for _ in range(n_queries)]
    prompts.sort(key=lambda pair: pair[1][1])  # (i, (kind, t, x)) --> kind
    t1 = time.time()
    solver = SolverSemiIntelligent()
    for i, (kind, t, value) in prompts:
        if kind == "p":
            answers[query_to_index[i]] = solver.query(t, value)
        elif kind == "v":
            solver.insert(t, value)
        else:
            raise ValueError(":)")
    assert len(correct_answers) == n_queries
    for i, (correct, given) in enumerate(zip(correct_answers, answers)):
        if check and correct != given:
            raise ValueError(f"Wrong answer #{i}: {correct} != {given}")
    t2 = time.time()
    print(f"Solved in {t2 - t1:.2f} seconds")
    print("")
    if not check:
        with open(out_file, "w") as f:
            for a in answers:
                print(a, file=f)


def solve_bf(test_case):
    print(f"Solving {test_case}")
    in_file = f"{test_case}.in"
    out_file = f"{test_case}.out"
    prompts = list(enumerate(load_file(in_file, True)))

    answers = []
    check = os.path.exists(out_file)
    if check:
        correct_answers = load_file(out_file, False)
    else:
        print("Won't be checking. Will write the answers")
        correct_answers = []
    t1 = time.time()
    solver = SolverBF()
    for i, (kind, t, value) in tqdm.tqdm(prompts):
        if kind == "p":
            answers.append(solver.query(t, value))
        elif kind == "v":
            solver.insert(t, value)
        else:
            raise ValueError(":)")
    for i, (correct, given) in enumerate(zip(correct_answers, answers)):
        if correct != given:
            raise ValueError(f"Wrong answer #{i}: {correct} != {given}")
    t2 = time.time()
    print(f"Solved in {t2 - t1:.2f} seconds")
    print("")
    if not check:
        with open(out_file, "w") as f:
            for a in answers:
                print(a, file=f)


def solve_all():
    test_cases = [naloga[:-3] for naloga in os.listdir(".") if naloga.endswith(".in")]
    for test_case in test_cases:
        solve_bf(test_case)
        if test_case.startswith("nalogaA"):
            solve_easy(test_case)


if __name__ == '__main__':
    solve_all()
