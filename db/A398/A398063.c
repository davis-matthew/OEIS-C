/*
 * A398063  Antidiagonal sums of the array A398062.
 * https://oeis.org/A398063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A(n+1-k, k), where A(r,k) = r-th nondivisor
 *            of k (nondivisors counted from 2)
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A398063_nth_nondivisor(idx_t k, idx_t r)
{
    idx_t x = r + 1;            /* candidate if no divisor were skipped */
    idx_t i;
    for (i = 2; i <= x; i++) {
        if (k % i == 0) x++;    /* divisor at or below x pushes the answer up */
    }
    return (seq_t)x;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    for (k = 1; k <= n; k++) {
        s += A398063_nth_nondivisor(k, n + 1 - k);
    }
    return s;
}
