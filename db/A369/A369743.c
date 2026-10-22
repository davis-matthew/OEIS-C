/*
 * A369743  a(n) = Sum_{p|n, p prime} p * Omega(n/p).
 * https://oeis.org/A369743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{p|n, p prime} p * Ω(n/p)
 *   For n > 1, this equals (Ω(n) - 1) * Σ_{p|n} p
 *   (where Ω(m) = total number of prime factors of m, counted with multiplicity).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;

    uint64_t tmp = (uint64_t)n;
    uint64_t sum_primes = 0;   /* Σ p_i over distinct prime divisors */
    uint64_t total_omega = 0;  /* Ω(n) = Σ e_i */

    /* handle factor 2 separately */
    if ((tmp & 1u) == 0) {
        sum_primes += 2;
        uint64_t e = 0;
        while ((tmp & 1u) == 0) {
            tmp >>= 1u;
            ++e;
        }
        total_omega += e;
    }

    /* trial division by odd numbers */
    for (uint64_t p = 3; p <= tmp / p; p += 2u) {
        if (tmp % p == 0) {
            sum_primes += p;
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            total_omega += e;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (tmp > 1) {
        sum_primes += tmp;
        total_omega += 1;
    }

    /* a(n) = (Ω(n) - 1) * Σ p_i */
    seq_t multiplicand = (seq_t)(total_omega - 1);
    seq_t scalar      = (seq_t)sum_primes;
    return multiplicand * scalar;
}
