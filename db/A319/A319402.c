/*
 * A319402  Number of partitions of n into exactly nine positive Fibonacci
 *          numbers.
 * https://oeis.org/A319402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   Number of partitions of n into exactly nine positive Fibonacci
 *            numbers. The code implements the standard generating‑function
 *            approach: a(n) = coefficient of x^n in [x^n](∏_{F∈Fib}
 *            1/(1–x^F)) where the sum is taken over exactly 9 parts. This DP
 *            counts unordered partitions (order of summands does n...
 * Range:     u8 n <= 232, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The external types seq_t and idx_t are supplied by the build. */

seq_t a(idx_t n)
{
    /* Generate list of distinct Fibonacci numbers ≤ n, starting with 1. */
    idx_t fib[20];          /* enough for n ≤ 10000 (≤20 values). */
    size_t fib_cnt = 0;

    /* Fibonacci sequence starts 1,1,2,3,5,...  We keep only unique values. */
    idx_t a1 = 1, a2 = 1;
    fib[fib_cnt++] = 1;       /* first distinct Fibonacci is 1 */

    while (1) {
        idx_t nxt = a1 + a2;
        if (nxt > n) break;
        /* skip duplicate 1 if it appears; after that all are distinct. */
        if (nxt != fib[fib_cnt - 1]) {
            fib[fib_cnt++] = nxt;
        }
        a1 = a2;
        a2 = nxt;
    }

    /* Allocate DP array: dp[k][s] for k=0..9, s=0..n. */
    size_t rows = 10;          /* k from 0 to 9 inclusive. */
    size_t cols = (size_t)n + 1;
    size_t total = rows * cols;
    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (dp == NULL) return 0;          /* allocation failure – safety. */
    for (size_t i = 0; i < total; ++i) dp[i] = 0;

    /* Helper macro for indexing. */
    #define IDX(k,s) (((k) * cols) + (s))

    dp[IDX(0,0)] = 1;   /* one way to partition 0 using 0 parts. */

    /* Perform DP: iterate over distinct Fibonacci values. */
    for (size_t fi = 0; fi < fib_cnt; ++fi) {
        idx_t f = fib[fi];
        for (size_t s = (size_t)f; s <= (size_t)n; ++s) {
            for (size_t k = 1; k <= 9; ++k) {
                dp[IDX(k,s)] += dp[IDX(k-1, s - f)];
            }
        }
    }

    seq_t result = dp[IDX(9, n)];
    free(dp);
    #undef IDX
    return result;
}
