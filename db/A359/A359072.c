/*
 * A359072  Denominators of the partial sums of the reciprocals of the maximal
 *          exponent in prime factorization of the positive integers
 *          (A051903).
 * https://oeis.org/A359072
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( Σ_{k=2..n} 1 / A051903(k) )
 * Range:     u8 n <= 23, u16 n <= 482, u32 n <= 9295, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"

/* Compute the denominator of the partial sum of reciprocals of A051903. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* Offset is 2, values below it are unused */

    /* Build smallest prime factor table up to n. */
    idx_t *spf = NULL;
    build_spf(n, &spf);             /* spf[0] = 0, spf[1] = 0, spf[i] = smallest prime factor of i */

    seq_t num = 0;                  /* Current numerator of the sum */
    seq_t den = 1;                  /* Current denominator of the sum */

    for (idx_t i = 2; i <= n; ++i) {
        /* Find the maximal exponent in the prime factorisation of i. */
        idx_t k = i;
        idx_t max_e = 0;
        while (k > 1) {
            idx_t p = spf[k];
            idx_t cnt = 0;
            while (k % p == 0) {
                k /= p;
                ++cnt;
            }
            if (cnt > max_e) max_e = cnt;
        }

        /* Add the term 1/max_e to the running sum: */
        seq_t e = (seq_t)max_e;
        seq_t new_num = num * e + den;
        seq_t new_den = den * e;
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;
        num = new_num;
        den = new_den;
    }

    free(spf);                      /* Clean up the SPF array */
    return den;                     /* The required denominator */
}
