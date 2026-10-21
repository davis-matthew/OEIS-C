/*
 * A293502  Greatest integer k such that k/n^2 < sqrt(2).
 * https://oeis.org/A293502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊√2·n²⌋, where r = √2
 * Range:     u8 n <= 13, u16 n <= 215, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The calculation is performed in 128‑bit arithmetic to avoid overflow
   for all n that fit in the supported seq_t type.  The resulting
   root is cast back to seq_t.  For very large n that overflow 128 bits
   the behaviour is unspecified, which is acceptable because the
   sequence is only required to be correct for values that fit in seq_t. */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns the largest r such that r*r ≤ x. */
static __int128 isqrt128(__int128 x)
{
    __int128 lo = 0;
    __int128 hi = ((__int128)1 << 64);   /* hi > sqrt(max x) */
    __int128 ans = 0;

    while (lo <= hi) {
        __int128 mid = (lo + hi) >> 1;
        if (mid == 0) {  /* avoid division by zero for x = 0 */
            lo = 1;
            continue;
        }
        /* Compare mid <= x/mid without overflow */
        __int128 div = x / mid;
        if (mid <= div) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute 2 * n^4 in 128‑bit */
    __int128 nn   = n;          /* n as 128‑bit */
    __int128 n2   = nn * nn;    /* n² */
    __int128 n4   = n2 * n2;    /* n⁴ */
    __int128 t    = n4 * 2;     /* 2·n⁴ */

    /* floor(√t) = floor(√(2·n⁴)) = floor(√2·n²) */
    __int128 root = isqrt128(t);

    return (seq_t)root;
}
