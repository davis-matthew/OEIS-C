/*
 * A340950  Number of ways to write n as an ordered sum of 5 nonzero
 *          triangular numbers.
 * https://oeis.org/A340950
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered sums of exactly 5 non‑zero triangular
 *            numbers that equal n. This implementation uses dynamic
 *            programming
 * Range:     u8 n <= 38, u16 n <= 927, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 5 only. */
    if (n < 5) {
        return 0; /* undefined for smaller indices */
    }

    /* 1. Generate all triangular numbers t_k = k(k+1)/2 that are <= n. */
    idx_t max_k = 0;
    idx_t tn = n;                 /* local copy of n */
    for (idx_t k = 1;; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > tn) break;
        ++max_k;
    }

    /* 2. Allocate an array to hold the triangular values. */
    idx_t *tri = (idx_t *)malloc((size_t)max_k * sizeof(idx_t));
    if (!tri) return 0; /* memory allocation failure */
    for (idx_t k = 1; k <= max_k; ++k) {
        tri[k - 1] = k * (k + 1) / 2;
    }

    /* 3. Dynamic programming: cur[s] holds the number of ways to form
       sum s using the numbers processed so far. */
    seq_t *cur = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!cur || !next) {
        free(cur);
        free(next);
        free(tri);
        return 0; /* memory allocation failure */
    }

    cur[0] = 1;  /* one way to make sum 0 with zero terms */

    for (idx_t step = 0; step < 5; ++step) {
        /* Reset next array to zero. */
        for (idx_t i = 0; i <= n; ++i) next[i] = 0;

        /* Transition: add one more triangular number. */
        for (idx_t s = 0; s <= n; ++s) {
            if (cur[s] == 0) continue;
            for (idx_t j = 0; j < max_k; ++j) {
                idx_t ns = s + tri[j];
                if (ns > n) break;  /* tri is sorted */
                next[ns] += cur[s];
            }
        }

        /* Swap cur and next for the next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[n];

    /* Clean up. */
    free(cur);
    free(next);
    free(tri);

    return result;
}
