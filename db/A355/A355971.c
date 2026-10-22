/*
 * A355971  Numerator of the cross-ratio of the four primes p_n, p_{n+1},
 *          p_{n+2}, p_{n+3}, where p_n = prime(n).
 * https://oeis.org/A355971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ((pₙ-pₙ₊₂)/(pₙ-pₙ₊₃)) *
 *            ((pₙ₊₁-pₙ₊₃)/(pₙ₊₁-pₙ₊₂)) where pₙ is the n‑th prime and p₁ = 2.
 *            The function computes the numerator of the fully reduced
 *            cross‑ratio
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

/* Euclidean algorithm for unsigned 64‑bit integers */
static inline unsigned long long gcd_ull(unsigned long long a,
                                         unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Indices of the four consecutive primes */
    idx_t n0 = n;
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;
    idx_t n3 = n + 3;

    /* Retrieve the primes (uint32_t) */
    uint32_t p0 = get_prime(n0);
    uint32_t p1 = get_prime(n1);
    uint32_t p2 = get_prime(n2);
    uint32_t p3 = get_prime(n3);

    /* Compute differences as 64‑bit values to avoid overflow */
    unsigned long long d_a = (unsigned long long)p0 - (unsigned long long)p2; /* pₙ - pₙ₊₂ */
    unsigned long long d_b = (unsigned long long)p0 - (unsigned long long)p3; /* pₙ - pₙ₊₃ */
    unsigned long long d_c = (unsigned long long)p1 - (unsigned long long)p3; /* pₙ₊₁ - pₙ₊₃ */
    unsigned long long d_d = (unsigned long long)p1 - (unsigned long long)p2; /* pₙ₊₁ - pₙ₊₂ */

    /* Numerator and denominator before simplification */
    unsigned long long num = d_a * d_c;
    unsigned long long den = d_b * d_d;

    /* Reduce by gcd and return the simplified numerator */
    unsigned long long g   = gcd_ull(num, den);
    unsigned long long n_reduced = num / g;

    return (seq_t)n_reduced;
}
