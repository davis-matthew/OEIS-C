/*
 * A275536  Differences of the exponents of the adjacent distinct powers of 2
 *          in the binary representation of n (with -1 subtracted from the
 *          least exponent present) are concatenated as decimal digits in
 *          reverse order.
 * https://oeis.org/A275536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) is obtained by listing the exponents of the set bits of n
 *            in * decreasing order, taking the successive differences (with
 *            the last * difference being the least exponent + 1), and then
 *            concatenating those * differences in reverse order as decimal
 *            digits
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 511, u64 n <= 511,
 *            u128 n <= 511, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, but guard anyway */

    /* collect exponents of set bits in ascending order */
    idx_t exps[64];
    unsigned int k = 0;
    idx_t pos = 0;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1)
            exps[k++] = pos;
        tmp >>= 1;
        ++pos;
    }

    /* compute differences in high–to–low order */
    seq_t diffs[64];
    unsigned int j = 0;
    if (k > 0) {
        for (int i = (int)k - 1; i >= 1; --i)
            diffs[j++] = (seq_t)(exps[i] - exps[i - 1]);
        diffs[j++] = (seq_t)(exps[0] + 1);
    }

    /* concatenate the differences in reverse order */
    seq_t result = 0;
    for (unsigned int idx = 0; idx < k; ++idx) {
        seq_t diff = diffs[k - 1 - idx];
        unsigned int len = digit_len(diff);
        seq_t mult = pow10((idx_t)len);
        result = result * mult + diff;
    }
    return result;
}
