from typing import List, Tuple
import random
import tqdm


class StatAlfaBeta:
    def __init__(self, values: List[int], alfa0: int, neutral_value: int):
        all_nodes, offset = StatAlfaBeta.find_full_tree_size(len(values))
        self.n_nodes = len(values)
        self.offset = offset
        self.nodes = [[alfa0, neutral_value] for _ in range(all_nodes)]
        self.n_leaves = [0 for _ in self.nodes]
        for i, x in enumerate(values):
            self.nodes[i + offset][0] = x
        for i_this in range(len(self.nodes) - 1, -1, -1):
            self.update_beta(i_this)

    def __str__(self):
        lines = [[]]
        n_this_line = 1
        processed_line = 0
        width = 1
        for alpha, beta in self.nodes:
            lines[-1].append(f"({alpha: 3}, {beta: 3})")
            processed_line += 1
            if n_this_line == processed_line:
                processed_line = 0
                n_this_line *= 2
                width = max(len(" ".join(lines[-1])), width)
                lines.append([])
        assert not lines[-1]
        lines.pop()
        # cca. curr_length + presl * L <= width
        for i in range(len(lines)):
            line_length = sum(map(len, lines[i]))
            n_spaces = len(lines[i])
            if i == len(lines) - 1:
                n_spaces -= 1
            space = " " * ((width - line_length) // n_spaces)
            lines[i] = space.join(lines[i]).center(width, " ")
        return "\n".join(lines)

    def update_beta(self, i: int):
        raise NotImplementedError()

    def get_value(self, i: int, j: int):
        raise NotImplementedError()

    def get_value_along_path(self, i, path, value_left_right, n_nodes_left_right):
        raise NotImplementedError()

    def get_paths_and_stuff(self, i: int, j: int):
        if not (0 <= i <= j < self.n_nodes):
            raise ValueError(f"Not true: 0 <= {i} <= {j} <= {self.n_nodes}")
        i_left = self.find_leaf(i)
        i_right = self.find_leaf(j)
        path_left = StatAlfaBeta.get_path_from_leaf(i_left)
        path_right = StatAlfaBeta.get_path_from_leaf(i_right)
        i_common, below_common = StatAlfaBeta.compute_common_and_paths_below(path_left, path_right)
        return i_left, i_right, path_left, path_right, i_common, below_common

    def increment(self, i: int, j: int, x: int):
        i_left, i_right, path_left, path_right, i_common, below_common = self.get_paths_and_stuff(i, j)
        internal_nodes = StatAlfaBeta.compute_internal_roots(*below_common)
        # increase alphas
        if i_left == i_right:
            self.nodes[i_left][0] += x
        else:
            for node in [i_left, i_right]:
                self.nodes[node][0] += x
        for root in internal_nodes:
            self.nodes[root][0] += x
        # adjust betas
        for root in internal_nodes:
            self.update_beta(root)
        found_common = False
        for left, right in zip(path_left, path_right):
            if left == i_common:
                found_common = True
            self.update_beta(left)
            if not found_common:
                self.update_beta(right)

    def find_leaf(self, i: int):
        return i + self.offset

    def n_leaves_in_subtree(self, i: int):
        if self.n_leaves[i] > 0:
            return self.n_leaves[i]
        i_left_leaf = i
        i_right_leaf = i
        # go left
        while 2 * i_left_leaf + 1 < len(self.nodes):
            i_left_leaf = 2 * i_left_leaf + 1
        # go right if possible, else go left
        while 2 * i_right_leaf + 1 < len(self.nodes):
            i_right_leaf = StatAlfaBeta.get_right(i_right_leaf)
        self.n_leaves[i] = i_right_leaf - i_left_leaf + 1
        return self.n_leaves[i]

    def is_internal(self, i: int):
        return 2 * i + 1 < len(self.nodes)

    def get_value_along_paths(self, i, j):
        i_left, i_right, path_left, path_right, i_common, below_common = self.get_paths_and_stuff(i, j)
        if i_left == i_right:
            final_value = 0  # add alpha later
        elif StatAlfaBeta.get_parent(i_left) == StatAlfaBeta.get_parent(i_right):
            final_value = self.nodes[i_left][1] + self.nodes[i_right][1]
        else:
            n_nodes_left_right = [0, 0]
            value_left_right = [0, 0]
            for i, path in enumerate(below_common):
                self.get_value_along_path(i, path, value_left_right, n_nodes_left_right)
            final_value = sum(value_left_right)
        n_nodes = i_right - i_left + 1
        return final_value, n_nodes, i_common

    @staticmethod
    def should_update_with_internal(i, path, j, node):
        root = -1
        update_with_internal = False
        if j == 0:
            root = node
        elif i == 0 and StatAlfaBeta.get_right(node) != path[j - 1]:
            # internal on left path
            root = StatAlfaBeta.get_right(node)
            update_with_internal = True
        elif i == 1 and StatAlfaBeta.get_left(node) != path[j - 1]:
            # internal on right
            root = StatAlfaBeta.get_left(node)
            update_with_internal = True
        return root, update_with_internal

    @staticmethod
    def get_path_from_leaf(i_leaf: int):
        current = i_leaf
        path = [current]
        while current:
            current = StatAlfaBeta.get_parent(current)
            path.append(current)
        return path

    @staticmethod
    def compute_common_and_paths_below(path_left: List[int], path_right: List[int]):
        assert len(path_left) == len(path_right)
        paths_bellow_common = [[], []]  # name misleading if i_common == 0
        paths_bellow_common[0].append(path_left[0])
        paths_bellow_common[1].append(path_right[0])
        i_common = -1
        for i, (left, right) in enumerate(zip(path_left, path_right)):
            if left == right:
                i_common = i
                break
        assert i_common >= 0
        if i_common > 0:
            paths_bellow_common[0].extend(path_left[1:i_common])
            paths_bellow_common[1].extend(path_right[1:i_common])
        return path_left[i_common], paths_bellow_common

    @staticmethod
    def compute_internal_roots(path_left: List[int], path_right: List[int]) -> List[int]:
        internal = []
        for i in range(1, len(path_left)):
            left = path_left[i]
            right = path_right[i]
            if StatAlfaBeta.get_left(left) == path_left[i - 1]:
                internal.append(StatAlfaBeta.get_right(left))
            if StatAlfaBeta.get_right(right) == path_right[i - 1]:
                internal.append(StatAlfaBeta.get_left(right))
        return internal

    @staticmethod
    def find_full_tree_size(n: int) -> Tuple[int, int]:
        depth = 1
        n_leaves = 1
        while n_leaves < n:
            n_leaves <<= 1
            depth += 1
        full_size = (1 << depth) - 1
        offset = full_size - (1 << depth - 1)
        return full_size, offset

    @staticmethod
    def get_parent(i: int):
        return (i - 1) >> 1

    @staticmethod
    def get_left(i: int):
        return 2 * i + 1

    @staticmethod
    def get_right(i: int):
        return 2 * i + 2


class SumOfRange(StatAlfaBeta):
    def __init__(self, values: List[int]):
        super().__init__(values, 0, 0)

    def update_beta(self, i: int):
        if self.is_internal(i):
            l_beta = self.nodes[StatAlfaBeta.get_left(i)][1]
            r_beta = self.nodes[StatAlfaBeta.get_right(i)][1]
            alfa = self.nodes[i][0]
            self.nodes[i][1] = l_beta + r_beta + self.n_leaves_in_subtree(i) * alfa
        else:
            self.nodes[i][1] = self.nodes[i][0]

    def get_value(self, i: int, j: int):
        final_value, n_nodes, i_common = self.get_value_along_paths(i, j)
        while i_common >= 0:
            final_value += self.nodes[i_common][0] * n_nodes
            i_common = StatAlfaBeta.get_parent(i_common)
        return final_value

    def get_value_along_path(self, i, path, value_left_right, n_nodes_left_right):
        for j, node in enumerate(path):
            root, update_with_internal = StatAlfaBeta.should_update_with_internal(i, path, j, node)
            if update_with_internal:
                value_left_right[i] += self.nodes[root][1]
            if root >= 0:
                n_nodes_left_right[i] += self.n_leaves_in_subtree(root)
            value_left_right[i] += self.nodes[node][0] * n_nodes_left_right[i]


class SumOfRangeBrute:
    def __init__(self, values: List[int]):
        self.values = values[:]

    def __str__(self):
        return " ".join(map(str, self.values))

    def increment(self, i, j, x):
        for k in range(i, j + 1):
            self.values[k] += x

    def get_value(self, i, j):
        return sum(self.values[i: j + 1])


class UnionSize(StatAlfaBeta):
    def update_beta(self, i: int):
        if self.nodes[i][0] > 0:
            self.nodes[i][1] = self.n_leaves_in_subtree(i)
        elif not self.is_internal(i):
            self.nodes[i][1] = 0
        else:
            l_beta = self.nodes[StatAlfaBeta.get_left(i)][1]
            r_beta = self.nodes[StatAlfaBeta.get_right(i)][1]
            self.nodes[i][1] = l_beta + r_beta

    def get_value(self, i: int, j: int):
        final_value, n_nodes, i_common = self.get_value_along_paths(i, j)
        while i_common >= 0:
            if self.nodes[i_common][0] > 0:
                return n_nodes  # won't change any more
            i_common = StatAlfaBeta.get_parent(i_common)
        return final_value

    def get_value_along_path(self, i, path, value_left_right, n_nodes_left_right):
        for j, node in enumerate(path):
            root, update_with_internal = StatAlfaBeta.should_update_with_internal(i, path, j, node)
            internal_value = self.nodes[root][1] if update_with_internal else 0
            if root >= 0:
                n_nodes_left_right[i] += self.n_leaves_in_subtree(root)
            if self.nodes[node][0] > 0:
                value_left_right[i] = n_nodes_left_right[i]
            else:
                value_left_right[i] += internal_value

    def __init__(self, values: List[int]):
        super().__init__(values, 0, 0)


class UnionSizeBrute:
    def __init__(self, values: List[int]):
        self.values = values[:]

    def __str__(self):
        return " ".join(map(str, self.values))

    def increment(self, i, j, x):
        for k in range(i, j + 1):
            self.values[k] += x
            if self.values[k] < 0:
                raise ValueError(f"{self.values}")

    def get_value(self, i, j):
        return sum(x > 0 for x in self.values[i: j + 1])


def test_sum_range():
    for n in range(13, 68):
        lst = list(range(n))
        t = SumOfRange(lst)
        b = SumOfRangeBrute(lst)
        for i in range(n):
            for j in range(i, n):
                t.increment(i, j, (i + 1) * (j + 1))
                b.increment(i, j, (i + 1) * (j + 1))
                if b.get_value(i, j) != t.get_value(i, j):
                    raise ValueError(f"{n}: [{i}, {j}]: {b.get_value(i, j)} != {t.get_value(i, j)}")


def test_union_size():
    """
    Vsak interval 3x doda, nato pa (v drugem vrstnem redu) 3x odstrani.
    Po vsakem dodajanju in odstranjevanju preveri.
    :return:
    """
    random.seed(1234)
    for n in range(27, 34):
        lst = [0 for _ in range(n)]
        t = UnionSize(lst)
        b = UnionSize(lst)
        intervals = [(i, j) for _ in range(3) for i in range(n) for j in range(i, n)]
        random.shuffle(intervals)
        for start, end in tqdm.tqdm(intervals):
            t.increment(start, end, 1)
            b.increment(start, end, 1)
            test_all_us(t, b, n)
        random.shuffle(intervals)
        for start, end in tqdm.tqdm(intervals):
            t.increment(start, end, -1)
            b.increment(start, end, -1)
            test_all_us(t, b, n)


def test_all_us(tree, brute, n):
    for i in range(n):
        for j in range(i, n):
            if brute.get_value(i, j) != tree.get_value(i, j):
                raise ValueError(f"{n}: [{i}, {j}]: {brute.get_value(i, j)} != {tree.get_value(i, j)}")


if __name__ == '__main__':
    test_sum_range()
    test_union_size()
