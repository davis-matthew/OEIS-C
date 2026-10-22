/*
 * A301317  a(n) = (n-1)! + 1 mod n^3.
 * https://oeis.org/A301317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)! + 1) mod n^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1704, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n >= 1.  For any n == 0 return 0. */
seq_t a(idx_t n)
{
    /* Floating-point and other libraries are not used. */
    if (n == 0)          /* safety, OEIS offset is 1 */
        return 0;

    /* n³ fits in seq_t as long as seq_t is large enough.  The product
       is performed in seq_t arithmetic so that it works for all seq_t
       sizes (uint8_t … big‑int).                                        */
    seq_t mod = (seq_t)n * n * n;    /* n³ */

    seq_t fact = 1;                   /* (n‑1)!  initialized at 0! = 1 */
    idx_t i;

#ifdef __cplusplus
    /* In C++ builds (arbitrary precision or 128‑bit seq_t) ordinary
       multiplication is safe because seq_t can hold the intermediate
       product.                                                      */
    for (i = 2; i < n; ++i)
        fact = (fact * (seq_t)i) % mod;
#else
    /* In C builds (uint64_t or smaller seq_t) use 128‑bit arithmetic
       to avoid overflow of the product a * i.                         */
    for (i = 2; i < n; ++i)
        fact = (seq_t)(((__int128)fact * i) % mod);
#endif

    /* (fact + 1) mod mod is the desired value.  The % operator
       correctly handles mod == 1 (e.g. n == 1).                         */
    return (fact + 1) % mod;
}
