/*
 * A319864  Exponents of the final nontrivial entry of the iterated Stern
 *          sequence; a(n) = log_2 min{s^k(n) : k > 0, s^k(n) > 1}, where s(n)
 *          = A002487(n).
 * https://oeis.org/A319864
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = log2(min{ s^k(n) : k > 0, s^k(n) > 1 }) where s(0)=0,
 *            s(1)=1 and for n > 1: if n even : s(n) = s(n/2) if n odd : s(n)
 *            = s((n-1)/2) + s((n+1)/2)
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   stern_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/stern_idx.h"

/* Integer binary logarithm for powers of two (result fits in seq_t). */
static seq_t floor_log2(seq_t x)
{
    seq_t e = 0;
    while (x > 1) {
        x >>= 1;
        ++e;
    }
    return e;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n < 2)               /* undefined for offset < 2; return 0 */
        return 0;

    /* Minimum value > 1 seen during the orbit of n under s(n). */
    seq_t min_val = (seq_t)n;          /* start with n itself */

    idx_t val = stern_idx(n);          /* first iterate */
    while (val > 1) {
        seq_t sval = (seq_t)val;
        if (sval < min_val)
            min_val = sval;
        val = stern_idx(val);
    }

    return floor_log2(min_val);
}
