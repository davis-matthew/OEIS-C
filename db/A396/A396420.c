/*
 * A396420  a(n) = r(n,n), where r(1,k)=f(k)=floor(k*c), r(n,k)=f(r(n-1,k)),
 *          c=4/3.
 * https://oeis.org/A396420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396420.
   Formula: a(n) = r(n,n), where
   r(1,k) = floor(k * 4/3),
   r(n,k) = floor(r(n-1,k) * 4/3),
   c = 4/3, offset = 1.
   Computes a(n) by iterating r starting from k = n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    // n >= 1 (offset 1)
    if (n == 0) return 0;          /* safety for non‑positive indices */

    /* start with r(1,n) = floor(n * 4/3) */
    seq_t x = ((seq_t)n * 4) / 3;
    /* apply the function (x * 4 / 3) n-1 more times */
    for (idx_t i = 2; i <= n; ++i) {
        x = (x * 4) / 3;
    }
    return x;
}
