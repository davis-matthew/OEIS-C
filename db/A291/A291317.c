/*
 * A291317  A variation of the Josephus problem: a(n) is the surviving integer
 *          under the following elimination process. Arrange 1,2,3,...,n in a
 *          circle, increasing clockwise. Starting with i=1, at k-th stage,
 *          move k places clockwise and delete the current number.
 * https://oeis.org/A291317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = survivor of a Josephus variant where at stage k we move k
 *            places clockwise and eliminate the current person
 * Range:     u8 n <= 268, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   find_kth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/find_kth.h"

/* Fenwick tree operations */
static inline void bit_add(int *bit, size_t n, size_t idx, int delta) {
    while (idx <= n) {
        bit[idx] += delta;
        idx += idx & -idx;
    }
}

/* Josephus variant with increasing steps */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    size_t N = (size_t)n;

    /* build Fenwick tree for all alive (value 1) */
    int *bit = (int *)malloc((N + 1) * sizeof(int));
    if (!bit) return (seq_t)0;
    for (size_t i = 1; i <= N; ++i) bit[i] = (int)(i & -i);

    size_t pos_rank = 1; /* rank of the current person in the alive list */

    for (size_t k = 1; k < N; ++k) {
        size_t sz = N - (k - 1);          /* remaining alive at this stage */
        size_t steps = k % sz;
        if (steps == 0) steps = sz;       /* delete current when k is a multiple of sz */

        size_t target_rank = ((pos_rank - 1 + steps) % sz) + 1;

        size_t idx = find_kth(bit, N, target_rank); /* 1‑based original index */
        bit_add(bit, N, idx, -1);                   /* eliminate */

        size_t sz_new = sz - 1;
        if (sz_new == 0) break;                   /* only for completeness */

        /* next current person: the one immediately after the deleted one */
        if (target_rank <= sz_new)
            pos_rank = target_rank;
        else
            pos_rank = 1;
    }

    size_t survivor_idx = find_kth(bit, N, 1);
    free(bit);
    return (seq_t)survivor_idx;
}
