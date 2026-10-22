/*
 * A327530  Number of divisors of n that are 1 or whose prime indices are
 *          relatively prime.
 * https://oeis.org/A327530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ d | n : gcd({ indices of prime factors of d (each
 *            repeated for its exponent) }) = 1 }|, where the index of a prime
 *            p is π(p). The divisor 1 is counted. This implementation
 *            enumerates all divisors of n via its prime factorisation and
 *            checks the gcd condition
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

/* Euclidean algorithm on idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Recursive helper to count valid divisors */
static seq_t _count_valid_divisors(const uint64_t primes[], const uint64_t exps[],
                                   size_t cnt, size_t pos, idx_t cur_gcd,
                                   const idx_t prime_idx[])
{
    if (pos == cnt) {
        /* divisor 1 has cur_gcd == 0, which we treat as valid */
        return (cur_gcd == 0 || cur_gcd == 1) ? (seq_t)1 : (seq_t)0;
    }

    seq_t total = 0;
    uint64_t max_e = exps[pos];
    idx_t idx = prime_idx[pos];

    for (uint64_t e = 0; e <= max_e; ++e) {
        idx_t new_gcd = cur_gcd;
        if (e > 0) {
            new_gcd = (cur_gcd == 0) ? idx : gcd_idx(cur_gcd, idx);
        }
        total += _count_valid_divisors(primes, exps, cnt, pos + 1, new_gcd, prime_idx);
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* No more than 64 distinct prime factors for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    if (cnt == 0) return (seq_t)1;           /* n == 1, already handled */

    /* Compute prime indices for the prime factors. */
    idx_t prime_idx[64];
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pi_val = compute_pi(primes[i]);   /* returns seq_t, fits in idx_t */
        prime_idx[i] = (idx_t)pi_val;
    }

    /* Count valid divisors via recursion. */
    return _count_valid_divisors(primes, exps, cnt, 0, (idx_t)0, prime_idx);
}
