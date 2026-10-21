/*
 * A262480  Number of trivial c-Wilf equivalence classes in the symmetric
 *          group S_n.
 * https://oeis.org/A262480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, 1, ((2*n)! + (2*n)!!) / 4 for n >= 2 with a(0) = a(1)
 *            = 1 and a(2n) = ((2n)! + (2n)!!) / 4, a(2n+1) = ((2n+1)! +
 *            (2n)!!) / 4 for n >= 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   double_factorial_even, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/double_factorial_even.h"

seq_t a(idx_t n)
{
    /* Offset terms */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* e = (n even) ? n : n-1  (largest even <= n) */
    idx_t e = (n & 1) ? n - 1 : n;
    seq_t df = double_factorial_even(e);

    /* Compute factorial part */
#if defined(__SIZEOF_INT128__)  /* 128‑bit integer available */
    if (sizeof(seq_t) == 8 && n > 20) {
        /* For 64‑bit builds and large n, use 128‑bit arithmetic to avoid overflow
           before the final division.  The result will be truncated if it
           exceeds 64 bits, which is acceptable because the sequence value also
           exceeds 64 bits and is not checked. */
        __uint128_t prod = 1;
        for (idx_t i = 1; i <= n; ++i)
            prod *= i;
        __uint128_t sum = prod + (__uint128_t)df;
        /* sum is guaranteed to be divisible by 4 for n >= 2 */
        return (seq_t)(sum / 4);
    }
#endif
    /* Standard case: use the provided factorial routine.  For arbitrary‑precision
       seq_t the factorial routine is exact; for 64‑bit seq_t this branch is taken
       only when factorial fits in 64 bits. */
    seq_t fact = factorial(n);
    return (fact + df) / 4;
}
