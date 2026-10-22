/*
 * A322358  Number of distinct twin prime pairs p, p+2 such that both of them
 *          divide n.
 * https://oeis.org/A322358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct twin prime pairs p, p+2 such that
 *            both primes divide n
 * Range:     u8 n <= 9660, u16 n <= 9660, u32 n <= 9660, u64 n <= 9660,
 *            u128 n <= 9660, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

/* Function to compute the A322358 sequence value for index n. */
seq_t a(idx_t n)
{
    /* Factorize n into its distinct prime divisors. */
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;

    factor_u64(n64, primes, exps, &cnt);

    /* Count twin prime pairs among the distinct prime divisors. */
    unsigned cnt_pairs = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            uint64_t diff = primes[j] > primes[i] ? primes[j] - primes[i]
                                                   : primes[i] - primes[j];
            if (diff == 2) {
                ++cnt_pairs;
            }
        }
    }

    return (seq_t)cnt_pairs;
}
