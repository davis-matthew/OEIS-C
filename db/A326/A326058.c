/*
 * A326058  a(n) = n - {the sum of square divisors of n}.
 * https://oeis.org/A326058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - sum of square divisors of n
 * Range:     i8 n <= 24, i16 n <= 9993, i32 n <= 9993, i64 n <= 9993,
 *            i128 n <= 9993, big any n
 * Helpers:   build_spf, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"   /* build_spf(idx_t limit, idx_t **spf_out) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Static SPF cache */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;

    /* Ensure SPF table covers n */
    if (n > spf_limit)
    {
        idx_t new_limit = n;
        if (spf_limit == 0) spf_limit = 2;
        while (spf_limit < new_limit) spf_limit <<= 1;   /* double until enough */

        if (spf) free(spf);
        build_spf(spf_limit, &spf);
    }

    /* Compute product of sums over prime exponents */
    idx_t tmp = n;
    seq_t sumsq = 1;   /* product over primes of (p^0 + p^2 + … + p^{2⌊e/2⌋}) */

    while (tmp > 1)
    {
        idx_t p = spf[tmp];
        idx_t e = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            e++;
        }

        idx_t k = e >> 1;   /* floor(e/2) */
        if (k == 0) continue;   /* term is 1, can skip */

        seq_t p2 = (seq_t)p * (seq_t)p;                 /* p² */
        seq_t num = seq_pow(p2, k + 1) - (seq_t)1;      /* p²^(k+1) – 1 */
        seq_t denom = p2 - (seq_t)1;                    /* p² – 1 */
        seq_t term = num / denom;                       /* exact division */
        sumsq *= term;
    }

    return (seq_t)n - sumsq;
}
