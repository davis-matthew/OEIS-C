/*
 * A359071  Numerators of the partial sums of the reciprocals of the maximal
 *          exponent in prime factorization of the positive integers
 *          (A051903).
 * https://oeis.org/A359071
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(∑_{k=2..n} 1/A051903(k))
 * Range:     u8 n <= 15, u16 n <= 206, u32 n <= 8404, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* cache for smallest prime factors */
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;
    if (n > spf_limit) {
        if (spf) free(spf);
        build_spf(n, &spf);
        spf_limit = n;
    }

    seq_t num = 0;
    seq_t den = 1;

    for (idx_t k = 2; k <= n; ++k) {
        /* compute A051903(k): maximum exponent in its prime factorisation */
        idx_t t = k;
        seq_t max_exp = 1;
        while (t > 1) {
            idx_t p = spf[t];
            seq_t cnt = 0;
            while (t % p == 0) {
                t /= p;
                cnt++;
            }
            if (cnt > max_exp) max_exp = cnt;
        }

        /* add the term 1/max_exp to the running sum */
        seq_t new_num = num * max_exp + den;
        seq_t new_den = den * max_exp;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;   /* numerator of the reduced fraction */
}
