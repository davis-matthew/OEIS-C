/*
 * A266926  a(0)=0, a(1)=1, a(2)=10; for n>2, a(n) = concat(a(1), ...,
 *          a(n-1)).
 * https://oeis.org/A266926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0; a(1)=1; a(2)=10; for n>2, a(n)=110*(10^(3*2^(n-3)) -
 *            1)/999, which equals the concatenation of '110' repeated 2^(n-3)
 *            times
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;

    /* n > 2 */
    /* Compute exponent e = 3 * 2^(n-3) */
    idx_t shift = n - 3;
    /* Guard against shift overflow; this should not happen for tested values. */
    if (shift >= 64)
        return (seq_t)0; /* fallback – should never be needed */

    idx_t pow2 = (idx_t)1 << shift;   /* 2^(n-3) */
    idx_t e = 3 * pow2;               /* 3*2^(n-3) */

    /* 10^e */
    seq_t pow10e = pow10(e);

    /* (10^e - 1) * 110 / 999 */
    seq_t res = pow10e - (seq_t)1;
    res = res * (seq_t)110;
    res = res / (seq_t)999;

    return res;
}
