from typing import List, Tuple


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
            if self.is_internal(i_this):
                self.update_beta_from_children(i_this)
            else:
                self.nodes[i_this][1] = self.nodes[i_this][0]

    def __str__(self):
        lines = [[]]
        n_this_line = 1
        processed_line = 0
        width = 1
        for alpha, beta in self.nodes:
            lines[-1].append(f"({alpha: 2}, {beta: 2})")
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
            joiner = " " * ((width - sum(map(len, lines[i]))) // len(lines[i]))
            lines[i] = joiner.join(lines[i]).center(width, " ")
        return "\n".join(lines)

    def update_beta_from_children(self, i: int):
        raise NotImplementedError()

    def get_value(self, i: int, j: int):
        raise NotImplementedError()

    def increment(self, i: int, j: int, x: int):
        raise NotImplementedError()

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
            i_right_leaf = 2 * i_right_leaf + 1
            if i_right_leaf + 1 < len(self.nodes):
                i_right_leaf += 1
        self.n_leaves[i] = i_right_leaf - i_left_leaf + 1
        return self.n_leaves[i]

    def is_internal(self, i: int):
        return 2 * i + 1 < len(self.nodes)

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

    def update_beta_from_children(self, i: int):
        l_beta = self.nodes[StatAlfaBeta.get_left(i)][1]
        r_beta = self.nodes[StatAlfaBeta.get_right(i)][1]
        alfa = self.nodes[i][0]
        self.nodes[i][1] = l_beta + r_beta + self.n_leaves_in_subtree(i) * alfa

    def get_paths_and_stuff(self, i: int, j: int):
        if not (0 <= i <= j < self.n_nodes):
            raise ValueError(f"Not true: 0 <= {i} <= {j} <= {self.n_nodes}")
        i_left = self.find_leaf(i)
        i_right = self.find_leaf(j)
        path_left = StatAlfaBeta.get_path_from_leaf(i_left)
        path_right = StatAlfaBeta.get_path_from_leaf(i_right)
        i_common, below_common = StatAlfaBeta.compute_common_and_paths_below(path_left, path_right)
        return i_left, i_right, path_left, path_right, i_common, below_common

    def get_value(self, i: int, j: int):
        i_left, i_right, path_left, path_right, i_common, below_common = self.get_paths_and_stuff(i, j)
        if i_left == i_right:
            final_value = 0  # add alpha later
            n_nodes = 1
        elif StatAlfaBeta.get_parent(i_left) == StatAlfaBeta.get_parent(i_right):
            final_value = self.nodes[i_left][1] + self.nodes[i_right][1]
            n_nodes = 2
        else:
            n_nodes_left_right = [0, 0]
            value_left_right = [0, 0]
            for i, path in enumerate(below_common):
                self.get_value_along_path(i, path, value_left_right, n_nodes_left_right)
            final_value = sum(value_left_right)
            n_nodes = sum(n_nodes_left_right)
        while i_common >= 0:
            final_value += self.nodes[i_common][0] * n_nodes
            i_common = StatAlfaBeta.get_parent(i_common)
        return final_value

    def get_value_along_path(self, i, path, value_left_right, n_nodes_left_right):
        for j, node in enumerate(path):
            update_with_internal = False
            root = -1
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
            if update_with_internal:
                value_left_right[i] += self.nodes[root][1]
            if root >= 0:
                n_nodes_left_right[i] += self.n_leaves_in_subtree(root)
            value_left_right[i] += self.nodes[node][0] * n_nodes_left_right[i]

    def increment(self, i: int, j: int, x: int):
        i_left, i_right, path_left, path_right, i_common, below_common = self.get_paths_and_stuff(i, j)
        if i_left == i_right:
            self.nodes[i_left][0] += x  # increase only alpha
            increment = x
        elif StatAlfaBeta.get_parent(i_left) == StatAlfaBeta.get_parent(i_right):
            for node in [i_left, i_right]:
                for i in range(2):
                    self.nodes[node][i] += x
            increment = 2 * x
        else:
            increment_left_right = [0, 0]
            for i, path in enumerate(below_common):
                self.increment_along_path(i, path, increment_left_right, x)
            increment = sum(increment_left_right)
        while i_common >= 0:
            self.nodes[i_common][1] += increment
            i_common = StatAlfaBeta.get_parent(i_common)

    def increment_along_path(self, i, path, increment_left_right, x):
        for j, node in enumerate(path):
            increase_parent = True
            if j == 0:
                root = node
                increase_parent = False
            elif i == 0 and StatAlfaBeta.get_right(node) != path[j - 1]:
                # internal on left path
                root = StatAlfaBeta.get_right(node)
            elif i == 1 and StatAlfaBeta.get_left(node) != path[j - 1]:
                # ... on right
                root = StatAlfaBeta.get_left(node)
            else:
                root = -1
            # pass the total increment so far
            self.nodes[node][1] += increment_left_right[i]
            if root <= 0:
                continue
            # update the internal root ...
            n_leaves = self.n_leaves_in_subtree(root)
            self.nodes[root][0] += x
            self.nodes[root][1] += n_leaves * x
            if increase_parent:
                # ... and parent if this is not i_left or i_right
                self.nodes[node][1] += n_leaves * x
            increment_left_right[i] += n_leaves * x


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
    def update_beta_from_children(self, i: int):
        i_left = 2 * i + 1
        i_right = 2 * i + 2
        if self.nodes[i][0] > 0:
            self.nodes[i][1] = self.n_leaves_in_subtree(i)
        else:
            self.nodes[i][1] = self.nodes[i_left][1] + self.nodes[i_right][1]

    def get_value(self, i: int, j: int):
        pass

    def increment(self, i: int, j: int, x: int):
        pass

    def __init__(self, values: List[int]):
        super().__init__(values, 0, 0)


def test_sum_range():
    n = 100
    lst = list(range(n))
    t = SumOfRange(lst)
    b = SumOfRangeBrute(lst)
    for i in range(n):
        for j in range(i, n):
            t.increment(i, j, (i + 1) * (j + 1))
            b.increment(i, j, (i + 1) * (j + 1))
            if b.get_value(i, j) != t.get_value(i, j):
                raise ValueError(f"{n}: [{i}, {j}]: {b.get_value(i, j)} != {t.get_value(i, j)}")


if __name__ == '__main__':
    test_sum_range()
