/*
 * A340953  Number of ways to write n as an ordered sum of 8 nonzero
 *          triangular numbers.
 * https://oeis.org/A340953
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 8‑tuples (t1,…,t8) of positive
 *            triangular numbers with t1+…+t8 = n. A triangular number is
 *            k*(k+1)/2 with k ≥ 1
 * Range:     u8 n <= 18, u16 n <= 68, u32 n <= 1765, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are defined elsewhere. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* For indices below the offset the value is not defined;
       return 0 for safety. */
    if (n < 8) return 0;

    /* Size for arrays.  Use size_t for indices. */
    size_t N = (size_t)n;

    /* Generate all triangular numbers ≤ N. */
    size_t max_k = (size_t)((-1 + (size_t)(1 + 8 * N)) / 2); /* rough upper bound */
    size_t *tri = (size_t *)malloc(max_k * sizeof(size_t));
    size_t tcount = 0;
    for (size_t k = 1; ; ++k) {
        size_t t = k * (k + 1) / 2;
        if (t > N) break;
        tri[tcount++] = t;
    }

    /* Dynamic programming: ways[s] = number of ways to reach sum s
       with the current number of summands. */
    seq_t *ways = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *newWays = (seq_t *)calloc(N + 1, sizeof(seq_t));
    ways[0] = 1;                 /* base case: zero sum with zero summands */

    for (size_t step = 0; step < 8; ++step) {
        /* Reset newWays to zero. */
        for (size_t i = 0; i <= N; ++i) newWays[i] = 0;

        /* Transition: add one more triangular number. */
        for (size_t s = 0; s <= N; ++s) {
            seq_t cur = ways[s];
            if (cur == 0) continue;
            for (size_t i = 0; i < tcount; ++i) {
                size_t ns = s + tri[i];
                if (ns > N) break;
                newWays[ns] += cur;
            }
        }

        /* Swap pointers for next iteration. */
        seq_t *tmp = ways;
        ways = newWays;
        newWays = tmp;
    }

    seq_t result = ways[N];

    /* Clean up. */
    free(ways);
    free(newWays);
    free(tri);

    return result;
}
