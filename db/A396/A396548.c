/*
 * A396548  The number of coreful divisors of n that are not exponential
 *          unitary divisors of n.
 * https://oeis.org/A396548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A005361(n) – A278908(n) * = product over prime exponents
 *            e of e * minus product over prime exponents e of 2^ω(e) * where
 *            n = ∏ p^e and ω(e) is the number of distinct prime factors of e
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* factorize n: n = Π p_i^{e_i} */
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n64, primes, exps, &cnt);

    seq_t core_cnt = 1;   /* Π e_i  (each e_i choices for f_i ∈ [1, e_i]) */
    seq_t unit_cnt = 1;   /* Π 2^ω(e_i)  (unitary divisor choices for each e_i) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        core_cnt *= (seq_t)e;

        size_t omega_e = (size_t)count_distinct_primes(e);
        unit_cnt *= ((seq_t)1 << omega_e);
    }

    return core_cnt - unit_cnt;
}
