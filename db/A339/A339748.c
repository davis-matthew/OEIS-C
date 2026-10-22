/*
 * A339748  a(n) = (6^(valuation(n, 6) + 1) - 1) / 5.
 * https://oeis.org/A339748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6^(valuation(n,6)+1)-1)/5
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* valuation(n,6) = min(v2(n), v3(n)), the exponent of 6 dividing n */

#include <stdint.h>

static seq_t pow6(seq_t exp)
{
    seq_t r = 1;
    while (exp > 0) {
        r = r * 6;
        exp = exp - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* count powers of 2 dividing n */
    idx_t k = n;
    uint64_t v2 = 0;
    while ((k & 1ULL) == 0) {
        k >>= 1;
        v2++;
    }

    /* count powers of 3 dividing n */
    k = n;
    uint64_t v3 = 0;
    while (k % 3 == 0) {
        k /= 3;
        v3++;
    }

    /* valuation of 6 is the minimum of v2 and v3 */
    uint64_t v6 = (v2 < v3) ? v2 : v3;

    /* compute (6^(v6+1)-1)/5 */
    seq_t pow = pow6((seq_t)(v6 + 1));
    pow = pow - 1;
    pow = pow / 5;
    return pow;
}
