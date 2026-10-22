/*
 * A324848  Number of prime indices of n (counted with multiplicity) that
 *          divide n.
 * https://oeis.org/A324848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A324848: number of prime indices of n (with multiplicity) that divide n */
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t count = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Index of the prime p in the prime sequence */
        seq_t m_seq = prime_pi(p);
        uint64_t m = (uint64_t)m_seq;

        /* Count how many times this index divides n */
        for (uint64_t j = 0; j < e; ++j) {
            if (n % m == 0)
                ++count;
        }
    }
    return count;
}
