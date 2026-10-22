/*
 * A372603  The maximal exponent in the prime factorization of the powerful
 *          part of n.
 * https://oeis.org/A372603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximal exponent among prime factors of n with exponent
 *            >=2, otherwise 0. That is, find the largest e_i in the prime
 *            factorization n = ∏ p_i^e_i for which e_i ≥ 2; return e_i, or 0
 *            if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple trial division factorisation; suitable for n up to about 10^10
   and fast enough for required limits (~10^4 calls). */
static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n < 2) return n;
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                /* a(1) = 0 */
    uint64_t nn = (uint64_t)n;
    uint64_t maxexp = 0;
    while (nn > 1) {
        uint64_t p = smallest_prime_factor(nn);
        uint64_t exp = 0;
        while (nn % p == 0) {
            nn /= p;
            exp++;
        }
        if (exp >= 2 && exp > maxexp)
            maxexp = exp;
    }
    return (seq_t)maxexp;
}
