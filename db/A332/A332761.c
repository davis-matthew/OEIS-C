/*
 * A332761  Exponents m such that the number of nonnegative k <= n, possessing
 *          the property that n + n*k - k is a square, is equal to 2^m.
 * https://oeis.org/A332761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the exponent m such that the number of k in [0,n] with *
 *            n + k*(n-1) a perfect square equals 2^m. The function counts
 *            such * perfect squares and returns floor(log2(count)). Because
 *            count is * always a power of two for the defined sequence, this
 *            gives the * required exponent
 * Range:     u8 n <= 17, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline uint32_t isqrt_uint32(uint32_t x) {
    uint32_t r = 0;
    for (int i = 15; i >= 0; --i) {       /* enough for x <= 1e9 */
        uint32_t bit = 1u << i;
        uint32_t tmp = r + bit;
        if (tmp * tmp <= x) r = tmp;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* a(0) = 0 by definition */

    uint64_t count = 0;
    seq_t tn = (seq_t)n;
    seq_t tn_minus1 = tn - 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t val = tn + (seq_t)k * tn_minus1;   /* n + k*(n-1) */
        uint64_t v64 = (uint64_t)val;            /* safe: val <= n*n <= 1e8 */
        uint32_t sqrtv = isqrt_uint32((uint32_t)v64);
        if ((uint64_t)sqrtv * sqrtv == v64) {
            ++count;
        }
    }

    /* count is guaranteed to be a power of two; find exponent */
    uint64_t exp = 0;
    uint64_t tmp = count;
    while (tmp > 1) {
        tmp >>= 1;
        ++exp;
    }
    return (seq_t)exp;
}
