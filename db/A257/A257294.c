/*
 * A257294  The first d decimal digits of the geometric mean of the digits of
 *          n, where d is the number of digits of n, without leading zeros.
 * https://oeis.org/A257294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(A007954(n)^(1/A055642(n))*10^(A055642(n)-1))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   digit_product, digits_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the first d decimal digits of the geometric mean of the digits of n,
   where d is the number of decimal digits of n. */

#include <stdint.h>
#include "../utils/digits_of.h"
#include "../utils/digit_product.h"

static inline unsigned __int128 pow_uint128(unsigned __int128 base, int exp)
{
    unsigned __int128 res = 1;
    while (exp-- > 0) {
        res *= base;
    }
    return res;
}

static inline unsigned __int128 pow10_uint128(unsigned int exp)
{
    unsigned __int128 res = 1;
    while (exp-- > 0) {
        res *= 10;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t prod = digit_product((seq_t)n);

    if (prod == 0) {
        return (seq_t)0;
    }

    int d = digits_of(n);
    if (d <= 0) {          /* should not occur, but be safe */
        return (seq_t)0;
    }

    /* limit = prod * 10^((d-1)*d) */
    unsigned __int128 limit = (unsigned __int128)prod;
    unsigned int exp = (unsigned int)((d - 1) * d);
    limit *= pow10_uint128(exp);

    /* search for the largest X such that X^d <= limit */
    unsigned __int128 low = 0;
    unsigned __int128 high = pow10_uint128(d) - 1;   /* 10^d - 1 */

    while (low < high) {
        unsigned __int128 mid = (low + high + 1) / 2;
        unsigned __int128 pow_mid = pow_uint128(mid, d);
        if (pow_mid <= limit) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }

    return (seq_t)low;
}
