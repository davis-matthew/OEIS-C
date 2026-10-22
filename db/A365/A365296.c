/*
 * A365296  The smallest coreful infinitary divisor of n.
 * https://oeis.org/A365296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} p^{2^{v2(e)}} (v2(e) = exponent of 2 in e)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* small power helper – calculates base^exp for relatively small exp */
static inline seq_t pow_seq_int(seq_t base, unsigned exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        if (exp >>= 1U) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* a(1)=1 */

    /* factor that number – primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t    cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];

        /* find v2(e) – number of trailing zeros in binary of e */
        unsigned v2 = 0;
        uint64_t temp = e;
        while ((temp & 1ULL) == 0ULL) {
            ++v2;
            temp >>= 1;
        }

        /* minimal infinitary exponent = 2^v2(e)  */
        uint64_t min_exp = 1ULL << v2;

        /* multiply term into result */
        seq_t term = pow_seq_int((seq_t)primes[i], (unsigned)min_exp);
        res *= term;
    }
    return res;
}
