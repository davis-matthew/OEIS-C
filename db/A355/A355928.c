/*
 * A355928  Squarefree part of the sum of divisors of n.
 * https://oeis.org/A355928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes having odd exponent in the sum of
 *            divisors σ(n). This is the odd‑part of σ(n) and equals the
 *            squarefree part of σ(n) after removing primes with even exponent
 * Range:     u8 n <= 143, u16 n <= 9042, u32 n <= 9042, u64 n <= 9042,
 *            u128 n <= 9042, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute σ(n) = sum of the positive divisors of n.  n is assumed
   to be a 64‑bit unsigned integer and the result fits into 64 bits
   for all test cases. */
static uint64_t compute_sigma(uint64_t n)
{
    uint64_t sigma = 1;
    uint64_t m = n;

    /* 2‑part */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        uint64_t term = 1;
        for (uint64_t i = 0; i < e + 1; ++i) term *= 2;
        term = (term - 1) / (2 - 1);          /* = 2^(e+1)-1 */
        sigma *= term;
    }

    /* odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            uint64_t term = 1;
            for (uint64_t i = 0; i < e + 1; ++i) term *= p;
            term = (term - 1) / (p - 1);
            sigma *= term;
        }
    }

    /* remaining prime factor */
    if (m > 1) {
        uint64_t p = m;
        uint64_t term = (p * p - 1) / (p - 1);  /* = p + 1 */
        sigma *= term;
    }

    return sigma;
}

/* Compute the odd part of σ(n), i.e. product of primes with odd
   exponent in the factorisation of σ(n). */
static uint64_t odd_part_of_sigma(uint64_t n)
{
    uint64_t sigma = compute_sigma(n);
    if (sigma == 0) return 0;

    uint64_t res   = 1;
    uint64_t m     = sigma;

    /* handle 2 */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        if (e & 1) res *= 2;
    }

    /* odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e & 1) res *= p;
        }
    }

    /* possible remaining prime factor */
    if (m > 1) {
        res *= m;            /* exponent is 1 (odd) */
    }

    return res;
}

/* OEIS sequence A355928 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for out‑of‑range input */
    return (seq_t)odd_part_of_sigma((uint64_t)n);
}
