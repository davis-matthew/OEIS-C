/*
 * A245547  Write n>=1 as either n=2^k-2^r with 0 <= r <= k-1, in which case
 *          a(2^k-2^r)=(k-r)*(k-r+1)/2, or as n=2^k-2^r+j with 2 <= r <= k-1,
 *          1 <= j < 2^r-1, in which case a(2^k-2^r+j)=(k-r)*(k-r+1)*a(j).
 * https://oeis.org/A245547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A245547:  a(1) = 1, 
 * a(n) = (k-r)*(k-r+1)/2                     if n = 2^k - 2^r           (0 ≤ r ≤ k-1)
 * a(n) = (k-r)*(k-r+1) * a(j)                 if n = 2^k - 2^r + j
 *                               (2 ≤ r ≤ k-1, 1 ≤ j < 2^r-1)
 * Here k is the unique integer with 2^(k-1) ≤ n < 2^k.
 */
#include <stdint.h>
#include <stdbool.h>

/* recursive helper: compute a(n) */
static seq_t
calc_a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* k : smallest integer with 2^k > n  (so 2^(k-1) ≤ n < 2^k) */
    idx_t k = 63 - __builtin_clzll(n) + 1;          /* floor(log2(n)) + 1 */
    idx_t P = ((idx_t)1) << k;                     /* 2^k */
    idx_t d = P - n;                               /* d = 2^k - n */

    /* Case 1: d is a power of two (first line of the definition) */
    if ((d & (d - 1)) == 0) {
        idx_t r = 63 - __builtin_clzll(d);         /* log2(d) */
        idx_t diff = k - r;
        return (seq_t)diff * ((seq_t)diff + 1) / 2;
    }

    /* Case 2: d is not a power of two */
    idx_t r = 63 - __builtin_clzll(d) + 1;         /* smallest r with 2^r > d (r ≥ 2) */
    idx_t pow = ((idx_t)1) << r;                   /* 2^r */
    idx_t j = pow - d;                             /* 1 ≤ j < 2^r-1 */

    seq_t factor = (seq_t)(k - r) * ((seq_t)(k - r) + 1); /* (k-r)*(k-r+1) */
    return factor * calc_a(j);
}

seq_t a(idx_t n)
{
    return calc_a(n);
}
