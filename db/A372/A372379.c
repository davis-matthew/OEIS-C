/*
 * A372379  The largest divisor of n whose number of divisors is a power of 2.
 * https://oeis.org/A372379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(p^e) = p^(2^floor(log_2(e+1))-1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 – return 1 for n == 1 */
    if (n <= 1)
        return (seq_t)1;

    uint64_t x = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(x, pr, ex, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        unsigned k = floor_log2_uint64(e + 1);
        uint64_t e_pow = ((uint64_t)1 << k) - 1;

        seq_t pow_term = big_pow((seq_t)pr[i], (seq_t)e_pow);
        result *= pow_term;
    }

    return result;
}
