/*
 * A326567  Numerator of the average of the multiset of prime indices of n.
 * https://oeis.org/A326567
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the average of the multiset of prime indices
 *            of n
 * Range:     u8 n <= 1618, u16 n <= 8500, u32 n <= 8500, u64 n <= 8500,
 *            u128 n <= 8500, big any n
 * Helpers:   factor_u64, gcd, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/prime_pi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* undefined for indices below the offset */

    /* Factor n: primes[ ] holds distinct prime factors, exps[ ] holds their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    seq_t total_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t idx = prime_pi((idx_t)primes[i]);  /* π(p) */
        seq_t e   = (seq_t)exps[i];
        sum      += idx * e;
        total_exp += e;
    }

    seq_t g = gcd(sum, total_exp);
    return sum / g;
}
