/*
 * A304831  a(n) = 123*2^n - 135.
 * https://oeis.org/A304831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 123*2^n - 135
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(1) = 123*2 - 135 = 111, matching the OEIS sequence */
    if (n == 0)
        return 0;                      /* undefined for offset 1, safety check */

    seq_t p = pow2(n);                  /* p = 2^n */
    return (seq_t)123 * p - (seq_t)135; /* a(n) = 123*2^n - 135 */
}
