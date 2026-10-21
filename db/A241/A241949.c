/*
 * A241949  Number of possible representations of n as a sum of distinct
 *          positive integers from the Fibonacci-type sequences
 *          0,1,1,2,3,5,8,13,... and 0,3,3,6,9,15,... (A000045 and A022086).
 * https://oeis.org/A241949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 2440, u32 n <= 9400, u64 n <= 9400,
 *            u128 n <= 9400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of subsets of the set
 *                {Fibonacci numbers, 3×Fibonacci numbers}
 *                that sum to n.
 *
 * The function uses a simple 0/1 knapsack DP that counts how many
 * distinct-element subsets of the set sum to each value up to n.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate a boolean array that marks which integers belong to the set.
       Use explicit casts from void* to satisfy both C and C++ compilers. */
    size_t len = (size_t)(n + 1);           /* array indices 0 .. n          */
    bool *present = (bool *)calloc(len, sizeof(bool));
    if (!present)
        return (seq_t)0;                    /* out of memory */

    /* Generate Fibonacci numbers <= n and mark them in the set. */
    idx_t fibs[90];                         /* more than enough for n <= 10000 */
    size_t fibcnt = 0;
    idx_t f0 = 0, f1 = 1;
    while (f1 <= n) {
        present[(size_t)f1] = true;          /* add F_i to the set           */
        fibs[fibcnt++] = f1;                 /* remember for 3*F_i           */
        if (f0 > UINT64_MAX - f1)            /* avoid overflow, though not needed for n <= 10000 */
            break;
        idx_t fnext = f0 + f1;               /* next Fibonacci number        */
        f0 = f1;
        f1 = fnext;
    }

    /* Add 3 × Fibonacci numbers to the set, without exceeding n. */
    for (size_t i = 0; i < fibcnt; ++i) {
        idx_t val = 3 * fibs[i];
        if (val <= n)
            present[(size_t)val] = true;
    }

    /* DP array that counts subsets summing to each value.
       Use a cast from void* for initialization. */
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) {
        free(present);
        return (seq_t)0;                    /* out of memory */
    }

    /* Explicitly initialize to zero in case seq_t is a class type with
       no trivial zero-initialization. */
    for (size_t i = 0; i < len; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                        /* empty sum */

    /* 0/1 knapsack update: iterate over every element of the set. */
    for (size_t w = 1; w <= (size_t)n; ++w) {
        if (!present[w])
            continue;                       /* not in the set */
        for (size_t sum = (size_t)n; sum >= w; --sum) {
            dp[sum] = dp[sum] + dp[sum - w];
            if (sum == w)                    /* avoid unsigned underflow */
                break;
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(present);
    return result;
}
