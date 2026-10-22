/*
 * A307313  a(n) is the denominator of n/2^(length of the binary
 *          representation of n).
 * https://oeis.org/A307313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = 2^(floor(log2(n))+1 - v2(n))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v2(n) is the exponent of 2 in n */

#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t temp = n;
    unsigned v2 = 0;
    while ((temp & 1ULL) == 0ULL) {
        v2++;
        temp >>= 1;
    }
    unsigned l = floor_log2_uint64(n) + 1; /* length of binary representation */
    unsigned exp = l - v2;
    return pow2(exp);
}
