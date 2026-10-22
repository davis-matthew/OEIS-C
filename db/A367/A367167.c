/*
 * A367167  Let Product_{i = 1..omega(n)} p_i^e_i be the prime factorization
 *          of n. Then a(n) = Sum_{i = 1..omega(n)} Product_{j = 1..n}
 *          p_i^(e_((i+j-1) mod omega(n) + 1)).
 * https://oeis.org/A367167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s=0}^{omega(n)-1} prod_{i=1}^{omega(n)}
 *            p_i^{e_{(i+s) mod omega(n)}}, with a(1) = 0 (empty sum)
 * Range:     u8 n <= 39, u16 n <= 271, u32 n <= 2367, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* small helper to compute base^exp for seq_t values. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;               /* special case, no primes */

    /* factorization arrays - plenty of space for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt == 0)          /* n == 1, just in case */
        return 0;
    if (cnt == 1)          /* single prime factor --> a(n) = n */
        return (seq_t)n;

    size_t k = cnt;
    seq_t sum = 0;

    /* all cyclic shifts of the exponent multiset */
    for (size_t s = 0; s < k; ++s) {
        seq_t prod = 1;
        for (size_t i = 0; i < k; ++i) {
            size_t eidx = (i + s) % k;            /* rotated exponent index */
            seq_t term = pow_seq((seq_t)primes[i],
                                 (idx_t)exps[eidx]);
            prod *= term;
        }
        sum += prod;
    }
    return sum;
}
