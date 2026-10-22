/*
 * A360096  To get a(n), replace 0's in the binary expansion of n with (-1)
 *          and interpret the result in base n.
 * https://oeis.org/A360096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret the binary expansion of n, replacing 0‑bits by
 *            -1, as a base‑n numeral. Computed with Horner’s rule: a(n) =
 *            (((1 * n - 1) * n - 1) * n + 1) ... (most significant bit first)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 63, u64 n <= 256,
 *            u128 n <= 3183, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* the empty bit string gives a(0)=0 */
        return 0;

    /* Determine the number of bits in n (bit length) */
    idx_t bits = 0;
    idx_t tmp  = n;
    while (tmp > 0) {           /* n > 0 guarantees at least one bit */
        ++bits;
        tmp >>= 1;
    }

    seq_t result = 0;
    seq_t base   = (seq_t)n;    /* the base = n in the numeral system */

    /* Process bits from most significant to least significant */
    for (idx_t i = bits; i > 0; --i) {
        /* bit index i-1: 1 if it's a 1‑bit, otherwise 0 */
        if ((n >> (i - 1)) & 1)        /* 1‑bit → digit = +1 */
            result = result * base + 1;
        else                           /* 0‑bit → digit = -1 */
            result = result * base - 1;
    }

    return result;
}
