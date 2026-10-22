/*
 * A351604  a(n) = n^6 * Sum_{d^2|n} 1 / d^6.
 * https://oeis.org/A351604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^6 * Sum_{d^2|n} 1 / d^6 (multiplicative) a(p^e) = p^6 *
 *            (p^(6*e) - p^(6*floor((e-1)/2))) / (p^6 - 1) Implementation uses
 *            prime factorisation and 128‑bit temporaries to avoid overflow
 *            for 64‑bit builds
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1623,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* power of an unsigned 64‑bit integer base raised to an
   integer exponent, computed in 128‑bit to avoid overflow. */
static seq_t pow_u64(uint64_t base, int exp)
{
    unsigned __int128 res = 1;
    for (int i = 0; i < exp; ++i)
        res *= (unsigned __int128)base;
    return (seq_t)res;
}

/* evaluate the prime‑power term a(p^e) using 128‑bit intermediates */
static seq_t a_prime_power(uint64_t p, int e)
{
    /* p^6 */
    unsigned __int128 p6 = 1;
    for (int i = 0; i < 6; ++i)
        p6 *= (unsigned __int128)p;

    /* p^(6*e) */
    unsigned __int128 pow_e = 1;
    int exp_e = 6 * e;
    for (int i = 0; i < exp_e; ++i)
        pow_e *= (unsigned __int128)p;

    /* k = floor((e-1)/2),  p^(6*k) */
    int k = (e - 1) / 2;
    unsigned __int128 pow_k = 1;
    int exp_k = 6 * k;
    for (int i = 0; i < exp_k; ++i)
        pow_k *= (unsigned __int128)p;

    /* numerator = p^6 * (p^(6e) - p^(6k)) */
    unsigned __int128 numer = p6 * (pow_e - pow_k);

    /* denominator = p^6 - 1 */
    unsigned __int128 denom = p6 - 1;

    /* integer division */
    unsigned __int128 res = numer / denom;
    return (seq_t)res;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)    /* safety guard – OEIS offset starts at 1 */
        return 0;

    /* prime factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= a_prime_power(primes[i], (int)exps[i]);

    return result;
}
