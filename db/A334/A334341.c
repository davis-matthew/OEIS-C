/*
 * A334341  a(n) = Sum_{p|n, p prime} (n - p).
 * https://oeis.org/A334341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*omega(n) - sopf(n) (sum over distinct prime factors of
 *            n)
 * Range:     u8 n <= 89, u16 n <= 9593, u32 n <= 9593, u64 n <= 9593,
 *            u128 n <= 9593, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined for n < 1, return 0 just in case */

    /* arrays to hold distinct prime factors and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 produces distinct primes in primes[0..cnt-1] */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t sopf = 0;   /* sum of distinct prime factors */
    uint64_t omega = 0;  /* number of distinct prime factors */

    for (size_t i = 0; i < cnt; ++i) {
        sopf += primes[i];
        ++omega;
    }

    /* compute n*omega - sopf using a type wide enough for all inputs */
    unsigned __int128 res = (unsigned __int128)n * omega - sopf;
    return (seq_t)res;
}
