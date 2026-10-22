/*
 * A365484  Square array read by ascending antidiagonals: T(n,k) is the k-th
 *          iterate of the 3x+1 function started at n, with n >= 1 and k >= 0.
 * https://oeis.org/A365484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 963, u16 n <= 9805, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A365484: a(n) is the value obtained by reading the 3x+1 iterate
   table T(n,k) (k-th iterate of the Collatz map starting at n)
   along ascending antidiagonals. */

#include <stdint.h>

/* Collatz step: if x is even, x/2; if x is odd, (3x+1)/2. */
static inline seq_t collatz_step(seq_t x)
{
    if ((x & 1) == 0)
        return x >> 1;
    else
        return (x * 3 + 1) >> 1;
}

/* Return the n-th term of OEIS A365484 (n >= 1). */
seq_t a(idx_t n)
{
    idx_t low = 1, high = 1;

    /* Find an upper bound for the diagonal index s. */
    while (1) {
        unsigned __int128 tri = (__uint128_t)high * (high + 1) / 2;
        if (tri >= n)
            break;
        high <<= 1;
    }

    /* Binary search for the smallest s such that s*(s+1)/2 >= n. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t s = low;                            /* diagonal number (1‑based) */
    unsigned __int128 prev128 = (__uint128_t)(s - 1) * s / 2;
    idx_t prev = (idx_t)prev128;              /* triangular number before this diagonal */
    idx_t offset = n - prev - 1;              /* zero‑based offset within the diagonal */
    idx_t row    = s - offset;                /* starting n in T(row,col) */
    idx_t col    = offset;                    /* number of iterations to apply */

    seq_t val = (seq_t)row;
    for (idx_t i = 0; i < col; ++i)
        val = collatz_step(val);

    return val;
}
