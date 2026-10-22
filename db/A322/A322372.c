/*
 * A322372  Least positive integer c such that c*prime(n) is practical.
 * https://oeis.org/A322372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer c such that c * prime(n) is
 *            practical
 * Range:     u8 n <= 54, u16 n <= 6809, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"     /* get_prime(idx_t k) -> uint32_t */
#include "../utils/factor_u64.h"    /* factor_u64(uint64_t n, uint64_t *pr,
                                    uint64_t *ex, size_t *cnt) */

/* Test whether a 64‑bit number is practical.  Returns 1 for practical,
   0 otherwise. */
static int is_practical_uint64(uint64_t m)
{
    /* Factor m; primes and exponents are stored in arrays. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    if (cnt == 0)           /* m == 1 – trivial case */
        return 1;
    if (pr[0] != 2)         /* first prime must be 2 */
        return 0;

    seq_t sigma_partial = 1;   /* product of σ(p^e) for processed primes */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        /* For i > 0 the prime p must satisfy: p <= σ_partial + 1   */
        if (i > 0) {
            seq_t limit = sigma_partial + (seq_t)1;
            if ((seq_t)p > limit)
                return 0;
        }

        /* Compute σ(p^e) = 1 + p + … + p^e */
        seq_t sigma_pe = 1;
        seq_t p_pow = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            p_pow = p_pow * (seq_t)p;
            sigma_pe += p_pow;
        }

        sigma_partial *= sigma_pe;
    }

    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 1, return 0 */
        return (seq_t)0;

    /* Get the n-th prime (n >= 1).  get_prime is 1‑based. */
    uint32_t prime32 = get_prime(n);
    uint64_t prime   = (uint64_t)prime32;

    /* Search for the smallest c such that c * prime is practical. */
    for (uint64_t c = 1; ; ++c) {
        uint64_t product = prime * c;          /* 64‑bit product – safe
                                                   for n up to 10 000      */
        if (is_practical_uint64(product))
            return (seq_t)c;
    }
}
