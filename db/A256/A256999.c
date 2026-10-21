/*
 * A256999  a(n)=n for n <= 1; for n >= 2, a(n) = largest number that can be
 *          obtained by rotating non-msb bits of binary expansion of n (with
 *          A080541 or A080542).
 * https://oeis.org/A256999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if n <= 1 a(n) = largest number obtained by rotating
 *            the lower bits of n (all bits below the most significant 1),
 *            keeping that 1 fixed, if n >= 2
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* small n */
    if (n <= 1) return (seq_t)n;

    /* compute bit length of n (len = floor_log2(n) + 1) */
    idx_t tmp = n;
    idx_t len = 0;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* number of bits below the leading 1 */
    idx_t m = len - 1;          /* m >= 1 here */

    /* the fixed most‑significant bit */
    idx_t msb = ((idx_t)1) << m;

    /* the lower m bits of n */
    idx_t bits = n ^ msb;       /* same as n - msb */

    /* mask to keep only m bits */
    idx_t mask = msb - 1;       /* (1<<m) - 1 */

    /* search for the maximum rotation of bits */
    idx_t maxRot = bits;
    for (idx_t k = 1; k < m; ++k) {
        idx_t rot = ((bits << k) | (bits >> (m - k))) & mask;
        if (rot > maxRot) maxRot = rot;
    }

    /* assemble the final result */
    idx_t result = msb | maxRot;

    return (seq_t)result;
}
