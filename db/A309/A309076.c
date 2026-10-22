/*
 * A309076  The Zeckendorf representation of n read as a NegaFibonacci
 *          representation.
 * https://oeis.org/A309076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 106, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A309076: for each n, find the Zeckendorf representation
   of n (sum of non‑consecutive Fibonacci numbers F_k, k ≥ 2),
   then compute a(n) = Σ (-1)^k * F_{k-1} for those k. */

#include <stdint.h>
#include <stddef.h>

#define MAX_FIB 200            /* enough for all test values (n ≤ ~10000) */

/* local Fibonacci array, recomputed per call */
static seq_t fib[MAX_FIB];

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    /* initialise Fibonacci sequence */
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;

    int idx = 2;
    /* build Fibonacci numbers up to the largest <= n */
    for (; idx < MAX_FIB; ++idx) {
        seq_t val = fib[idx - 1] + fib[idx - 2];
        if (val > (seq_t)n)
            break;
        fib[idx] = val;
    }
    /* idx is the first index where fib[idx] > n, so last valid index is idx-1 */
    int max_index = idx - 1;

    seq_t remaining = (seq_t)n;
    seq_t result   = (seq_t)0;

    /* greedy Zeckendorf decomposition */
    while (remaining > 0) {
        int i = max_index;
        while (fib[i] > remaining)
            --i;                       /* find largest Fibonacci ≤ remaining */

        /* contribution: (-1)^i * fib[i-1] */
        if (i & 1)
            result -= fib[i - 1];
        else
            result += fib[i - 1];

        remaining -= fib[i];
        max_index = i - 1;              /* next largest cannot use same index */
    }

    return result;
}
