/*
 * A389454  Arithmetic derivative of the largest unitary divisor of n that is
 *          a term in A048103 (not divisible by p^p for any prime p).
 * https://oeis.org/A389454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 233, u16 n <= 9494, u32 n <= 9494, u64 n <= 9494,
 *            u128 n <= 9494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): arithmetic derivative of the largest unitary divisor of n
   that is a term of A048103 (i.e., all prime exponents < the prime). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n <= 1ull) return (seq_t)0;   /* A(1)=0 */

    uint64_t tmp = (uint64_t)n;       /* work on a 64‑bit copy */
    seq_t d = 1;                      /* largest unitary divisor in A048103 */
    seq_t der = 0;                    /* result (arithmetic derivative) */

    /* maximum number of distinct prime factors for a 64‑bit integer is < 64 */
    uint64_t primes[64];
    unsigned  exps[64];
    size_t pcnt = 0;

    /* trial division factorisation */
    for (uint64_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            unsigned e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1)                      /* remaining prime factor */
    {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* collect indices of primes whose exponent satisfies e < p */
    size_t sel[64];
    size_t scnt = 0;

    /* build the product d of selected prime powers */
    for (size_t i = 0; i < pcnt; ++i)
    {
        uint64_t p = primes[i];
        unsigned e = exps[i];
        if (e < p)                    /* exponent satisfies the condition */
        {
            seq_t pow = 1;
            for (unsigned k = 0; k < e; ++k) pow *= (seq_t)p;
            d *= pow;
            sel[scnt++] = i;
        }
    }

    if (d == (seq_t)1) return (seq_t)0;   /* no prime satisfied the test */

    /* compute the arithmetic derivative: D(d) = d * Σ(e/p) over selected primes */
    for (size_t si = 0; si < scnt; ++si)
    {
        size_t i = sel[si];
        uint64_t p = primes[i];
        unsigned e = exps[i];
        seq_t term = d / (seq_t)p;    /* integer division */
        term *= (seq_t)e;
        der += term;
    }
    return der;
}
