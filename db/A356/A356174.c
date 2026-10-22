/*
 * A356174  Replace all prime factors p of n with n/p, then take the integer
 *          logarithm (A001414).
 * https://oeis.org/A356174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Ω(n) - 1) * Σ (e_i * p_i) where the sum runs over prime
 *            factors p_i of n with multiplicity e_i. This follows from the
 *            identities: a(n*m) = (L(n)+L(m)) * (Ω(n)+Ω(m)-1) with L(n)
 *            additive L(p) = p for primes, L(p^k) = k p Thus L(n) = Σ (e_i *
 *            p_i) and a(n) = L(n)*(Ω(n)-1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                /* a(1) = 0 by definition */
    uint64_t rem = (uint64_t)n;
    seq_t sum = 0;
    idx_t omega = 0;

    while (rem > 1) {
        uint64_t p = _smallest_prime_factor(rem);
        idx_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            e++;
        }
        sum += (seq_t)(e * p);
        omega += e;
    }

    if (omega <= 1) return 0;            /* primes and n=1 */
    return sum * (omega - 1);
}
