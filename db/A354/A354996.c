/*
 * A354996  a(n) = 1 if n > A034699(n)^2, otherwise 0, where A034699(n) is the
 *          largest prime power factor of n.
 * https://oeis.org/A354996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A034699(n)^2 < n]
 * Range:     u8 n <= 15, u16 n <= 7158, u32 n <= 9427, u64 n <= 9427,
 *            u128 n <= 9427, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A034699(n) is the largest prime power factor of n; we return 1 if n > (A034699(n))^2, else 0. */
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t largest_prime_power_uint64(uint64_t n)
{
    if (n <= 1) return (seq_t)1;          /* for n = 1, largest prime power is 1 */

    /* maximum distinct prime factors of a 64‑bit integer < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t max_pp = 1;                     /* start with 1 (for n = 1 case) */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pp = 1;
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        for (uint64_t k = 0; k < e; ++k)
            pp *= (seq_t)p;                /* multiply in seq_t arithmetic */
        if (pp > max_pp) max_pp = pp;
    }
    return max_pp;
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t gp = largest_prime_power_uint64((uint64_t)n);
    seq_t gp_sq = gp * gp;
    return (gp_sq < n_seq) ? (seq_t)1 : (seq_t)0;
}
