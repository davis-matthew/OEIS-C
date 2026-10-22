/*
 * A393351  a(n) is the lcm of n and all (p_i + e_i) in the canonical prime
 *          factorization of n.
 * https://oeis.org/A393351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393351:  a(n) = lcm of n and all (p_i + e_i) from the prime factorization of n */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for n=0, but guard against accidental calls */
    }
    if (n == 1) {
        return 1;
    }

    /* factorization of n: primes[i] ^ exps[i]  (i < count) */
    uint64_t primes[12];
    uint64_t exps[12];
    size_t count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t result = (seq_t)n;  /* start with n itself */

    for (size_t i = 0; i < count; ++i) {
        seq_t val = (seq_t)primes[i] + (seq_t)exps[i];
        seq_t g = gcd(result, val);
        result = (result / g) * val;   /* lcm(res, val) */
    }

    return result;
}
