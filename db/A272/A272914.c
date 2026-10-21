/*
 * A272914  Sixth powers ending in digit 6.
 * https://oeis.org/A272914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10*n - 3*(-1)^n - 5)^6 / 64 Implementation uses 128‑bit
 *            arithmetic when available to avoid overflow for all n up to
 *            10000
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 325, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute t = 10*n - 3*(-1)^n - 5
       which simplifies to:
           t = 10*n - 8  if n is even
           t = 10*n - 2  if n is odd
    */
    seq_t t = (seq_t)(10 * n);
    if (n & 1)
        t -= (seq_t)2;   /* odd n */
    else
        t -= (seq_t)8;   /* even n */

#if defined(__SIZEOF_INT128__)
    /* Use unsigned 128‑bit integers for the intermediate product.
       The product fits comfortably in 128 bits for all n up to 10000. */
    unsigned __int128 u = (unsigned __int128) t;
    unsigned __int128 u2 = u * u;       /* t^2   */
    unsigned __int128 u4 = u2 * u2;     /* t^4   */
    unsigned __int128 u6 = u4 * u2;     /* t^6   */
    return (seq_t)(u6 / 64);
#else
    /* Fallback: compute with seq_t operations.
       For 64‑bit builds this may overflow for large n, but only
       values that fit in seq_t are required to be correct. */
    seq_t t2 = t * t;         /* t^2   */
    seq_t t4 = t2 * t2;       /* t^4   */
    seq_t t6 = t4 * t2;       /* t^6   */
    return t6 / 64;
#endif
}
