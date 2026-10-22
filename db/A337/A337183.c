/*
 * A337183  If n is composite, a(n) is the number represented by the list of
 *          prime factors of n (with multiplicity, sorted in nondecreasing
 *          order) in base the sum of that list. If n is prime, a(n) = n.
 * https://oeis.org/A337183
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is n if n is prime; otherwise a(n) is the base-(sum of
 *            prime factors) integer whose digits are the prime factors of n
 *            (with multiplicity, sorted)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation relies on the provided utils:
 * - is_prime(idx_t)
 * - factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
 * The sequence values are computed using seq_t arithmetic. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)          /* out‑of‑range; not part of the official offset */
        return 0;
    if (is_prime(n))    /* prime case – return the number itself */
        return (seq_t)n;

    /* Factor the composite n. */
    uint64_t primes[64] = {0};
    uint64_t exps[64]   = {0};
    size_t   cnt        = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Expand the factorization into a list of primes (with multiplicity). */
    uint64_t pf[64] = {0};
    size_t   k     = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        size_t   e = exps[i];
        while (e--)
        {
            pf[k++] = p;   /* store the prime factor */
        }
    }

    /* Compute the base (sum of primes). */
    seq_t sum = 0;
    for (size_t i = 0; i < k; ++i)
        sum += (seq_t)pf[i];

    /* Interpret the list as a base‑sum integer. */
    seq_t res = 0;
    for (size_t i = 0; i < k; ++i)
        res = res * sum + (seq_t)pf[i];

    return res;
}
