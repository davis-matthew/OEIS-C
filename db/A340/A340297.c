/*
 * A340297  a(n) = (Sum of totatives of n) mod (Sum of primes dividing n with
 *          multiplicity).
 * https://oeis.org/A340297
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A023896(n) mod A001414(n)
 * Range:     u8 n <= 1534, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A023896(n) = sum of totatives of n (numbers ≤ n coprime to n)  */
/* A001414(n) = sum of prime factors of n with multiplicity.     */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    /* Undefined for n < 2; return 0 for safety. */
    if (n < 2) return 0;

    /* Factorise n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute Euler's totient φ(n).  Use 64‑bit arithmetic
       which is safe for n ≤ 100 000  (and easily for the limits). */
    uint64_t phi = (uint64_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        phi = (phi / p) * (p - 1);   /* exact division */
    }

    /* Sum of totatives: n * φ(n) / 2  (valid for n > 1). */
    uint64_t sum_totatives = ((uint64_t)n * phi) / 2;

    /* Sum of prime factors with multiplicity. */
    uint64_t sum_primes_mul = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum_primes_mul += primes[i] * exps[i];
    }

    /* Result is the residue of sum_totatives modulo sum_primes_mul. */
    uint64_t result = sum_totatives % sum_primes_mul;

    return (seq_t)result;   /* cast to seq_t; overflow is allowed for small seq_t builds */
}
