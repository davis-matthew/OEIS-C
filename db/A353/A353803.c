/*
 * A353803  a(n) = Product_{p^e||n} sigma(sigma(p^e)) - sigma(sigma(n)), where
 *          n = Product_{p^e||n}, with each p^e the maximal power of prime p
 *          that divides n.
 * https://oeis.org/A353803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p^e||n} sigma(sigma(p^e)) - sigma(sigma(n))
 * Range:     u8 n <= 65, u16 n <= 2309, u32 n <= 9682, u64 n <= 9682,
 *            u128 n <= 9682, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A simple trial‑division factoriser that stores the
   distinct prime factors and their exponents in an array. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"

/* Factorize n.  For n ≤ 10 000 a fixed buffer is sufficient. */
static void factor_small(uint64_t n, uint64_t *pr, int *exps, size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            pr[c]   = p;
            exps[c] = 0;
            while (m % p == 0) {
                m /= p;
                ++exps[c];
            }
            ++c;
        }
    }
    if (m > 1) {
        pr[c]   = m;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)  /* not used in OEIS but keep safe */
        return (seq_t)0;

    /* Use a small fixed buffer for the (at most) 16 distinct primes
       of numbers up to 10_000. */
    uint64_t primes[16];
    int exps[16];
    size_t cnt;

    factor_small((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* Compute p^e. */
        seq_t pe = 1;
        for (int j = 0; j < exps[i]; ++j)
            pe *= (seq_t)primes[i];

        /* sigma(p^e) and sigma(sigma(p^e)) */
        seq_t sigma_pe        = sigma_of((idx_t)pe);
        seq_t sigma_sigma_pe  = sigma_of((idx_t)sigma_pe);

        prod *= sigma_sigma_pe;
    }

    /* sigma(sigma(n)) */
    seq_t sigma_n       = sigma_of(n);
    seq_t sigma_sigma_n = sigma_of((idx_t)sigma_n);

    return prod - sigma_sigma_n;
}
