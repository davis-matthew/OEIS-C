/*
 * A374470  a(n) = gcd(bigomega(n), A064547(n)), where A064547 is the count of
 *          1-bits in the exponents of the prime factorization of n, and
 *          bigomega is the number of prime factors of n (with multiplicity).
 * https://oeis.org/A374470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(bigomega(n), A064547(n))
 * Range:     u8 n <= 8814, u16 n <= 8814, u32 n <= 8814, u64 n <= 8814,
 *            u128 n <= 8814, big any n
 * Helpers:   bigomega, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064547(n) = sum of popcount(e_i) over the exponents e_i in the prime factorization of n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                   /* a(1) = 0, and a(0) is undefined but return 0 */

    /* bigomega(n) */
    seq_t omega = bigomega(n);

    /* factor n and collect exponents */
    /* maximum number of distinct prime factors for a 64‑bit n is < 20, use a safe buffer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* sum popcount of each exponent */
    unsigned total = 0;
    for (size_t i = 0; i < cnt; ++i)
        total += (unsigned)__builtin_popcountll(exps[i]);

    seq_t bitcnt = (seq_t)total;

    /* return gcd of the two values */
    return gcd(omega, bitcnt);
}
