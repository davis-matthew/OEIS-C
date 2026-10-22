/*
 * A372692  The sum of infinitary divisors of the smallest number k such that
 *          k*n is a number whose number of divisors is a power of 2
 *          (A036537).
 * https://oeis.org/A372692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(p^e) = ∏_{k ≥ 0, 2^k < e, (e &
 *            2^k) = 0} (p^(2^k) + 1)
 * Range:     u8 n <= 1295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Handle n = 1 (no prime factors) */
    if (n == 0) return 0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt;

    factor_u64(nn, primes, exps, &prime_cnt);

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t prod = 1;
        for (uint64_t k = 1; k < e; k <<= 1) {
            if ((e & k) == 0) {
                seq_t term = pow_seq((seq_t)p, (int)k);
                term += 1;
                prod *= term;
            }
        }
        result *= prod;
    }

    return result;
}
