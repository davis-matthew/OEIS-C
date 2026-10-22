/*
 * A302205  Let the binary expansion of n be [b_d, b_{d-1}, ..., b_3, b_2,
 *          b_1, b_0]_2, where (if n>0) b_d = 1, b_i = 0 or 1 for i<d. To get
 *          a(n) concatenate the decimal numbers 2^(b_i) (if b_i = 1) or 0 (if
 *          b_i = 0).
 * https://oeis.org/A302205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate over the binary digits of n (from most
 *            significant to least). For each bit i (starting with the highest
 *            set bit d down to 0): if bit i is 1: append the decimal
 *            representation of 2^i, else : append "0" (a single zero digit).
 *            Equivalent to: for each bit i, multiply the current va...
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digit_len, pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Find position of the most significant set bit d. */
    idx_t d;
#if defined(__GNUC__) && defined(__builtin_clzll)
    d = 63 - __builtin_clzll(n);               /* n>0 ensures safety */
#else
    {
        idx_t tmp = n;
        d = 0;
        while (tmp >>= 1) {
            d++;
        }
    }
#endif

    seq_t result = 0;
    for (idx_t i = d; ; i--) {
        bool bit = (n >> i) & 1ULL;
        if (bit) {
            seq_t val = pow2(i);                 /* 2^i */
            idx_t k = (idx_t)digit_len(val);    /* number of decimal digits */
            seq_t mult = pow10(k);              /* 10^k */
            result = result * mult + val;
        } else {
            result = result * 10;                /* append single zero digit */
        }
        if (i == 0) {
            break;
        }
    }
    return result;
}
