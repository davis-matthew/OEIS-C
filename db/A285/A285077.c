/*
 * A285077  Positions of 0 in A285076; complement of A285078.
 * https://oeis.org/A285077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A285077: Positions of 0 in the binary word A285076.
   The sequence satisfies  
      a(1) = 2
      a(n) = n + ⌊ n/√2 ⌋ + 1   if ⌊ n/√2 ⌋ = ⌊ (n‑1)/√2 ⌋,
      a(n) = n + ⌊ n/√2 ⌋        otherwise
   (valid for all n ≥ 1).  
   Here ⌊ n/√2 ⌋ is computed as floor(sqrt(⌊n²/2⌋)) so that only
   integer arithmetic is used.
*/

#include <stdint.h>
#include <stdbool.h>

/* 128‑bit integer square root: returns ⌊√x⌋  (x ≤ 2^127) */
static seq_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;   /* sqrt(2^128-1) < 2^64 */

    while (low <= high) {
        unsigned __int128 mid   = (low + high) >> 1;
        unsigned __int128 mid2  = mid * mid;

        if (mid2 <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)high;        /* fits in seq_t (≤ 2^64‑1) */
}

seq_t a(idx_t n)
{
    /* offset 1: handle the first term */
    if (n == (idx_t)1) {
        return (seq_t)2;
    }

    unsigned __int128 n128   = (unsigned __int128)n;
    unsigned __int128 n1disk = n128 - 1;

    unsigned __int128 half_n  = (n128   * n128)   >> 1;   /* floor(n²/2) */
    unsigned __int128 half_n1 = (n1disk * n1disk) >> 1;   /* floor((n‑1)²/2) */

    seq_t a0 = isqrt128(half_n);   /* floor(n/√2) */
    seq_t a1 = isqrt128(half_n1);  /* floor((n‑1)/√2) */

    seq_t t = (seq_t)n + a0;                 /* floor(n*(1+1/√2)) */

    if (a0 == a1) {                         /* add 1 when the gap is 1 */
        t += (seq_t)1;
    }
    return t;
}
