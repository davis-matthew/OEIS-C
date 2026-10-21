/*
 * A277439  Numerators of a sequence defined by a modified recurrence for the
 *          exponential of the von Mangoldt function.
 * https://oeis.org/A277439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the reduced fraction S(n,1) where S(1,1)=1
 *            and for n>1 S(n,1) = n · Π_{j|(n+1), 2≤j≤n} (den[j]/num[j]) ·
 *            Π_{j|(n-1), 2≤j≤n} (den[j]/num[j]), with num[k]/den[k] = S(k,1)
 *            in reduced form. The sequence a(n) is the numerators of these
 *            fractions
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* static memoization arrays */
    static idx_t max_computed = 0;
    static seq_t *num = NULL;   /* numerator of S(k,1)   */
    static seq_t *den = NULL;   /* denominator of S(k,1) */

    /* allocate or extend storage if needed */
    if (!num || max_computed == 0) {
        /* first call – allocate up to n */
        num = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        den = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!num || !den) { abort(); }
        num[1] = den[1] = 1;     /* base case S(1,1) = 1 */
        max_computed = 1;
    } else if (n > max_computed) {
        /* grow arrays to the requested size */
        seq_t *tmp = (seq_t *)realloc(num, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        num = tmp;
        tmp = (seq_t *)realloc(den, sizeof(seq_t) * (n + 1));
        if (!tmp) abort();
        den = tmp;
    }

    /* compute terms up to n that are not yet computed */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 1) {    /* already initialized */
            continue;
        }

        seq_t num_k = (seq_t)k;   /* start with k */
        seq_t den_k = 1;          /* denominator 1 */

        /* process divisors of k+1 (set A)  */
        idx_t mp1 = k + 1;
        for (idx_t d = 2; d < mp1; ++d) {          /* d runs 2..k */
            if (mp1 % d == 0) {
                idx_t idx = mp1 / d;                /* < k */
                /* multiply by den[idx]/num[idx] */
                seq_t f_num = den[idx];             /* factor numerator  */
                seq_t f_den = num[idx];             /* factor denominator */

                seq_t g1 = seq_gcd(f_num, den_k);
                f_num /= g1;
                den_k  /= g1;

                seq_t g2 = seq_gcd(f_den, num_k);
                f_den /= g2;
                num_k  /= g2;

                seq_t new_num = num_k * f_num;
                seq_t new_den = den_k * f_den;
                seq_t g3 = seq_gcd(new_num, new_den);
                num_k = new_num / g3;
                den_k = new_den / g3;
            }
        }

        /* process divisors of k-1 (set B)  */
        if (k > 1) {
            idx_t mm1 = k - 1;
            for (idx_t d = 2; d < mm1; ++d) {       /* d runs 2..k-2 */
                if (mm1 % d == 0) {
                    idx_t idx = mm1 / d;                /* < k */
                    seq_t f_num = den[idx];
                    seq_t f_den = num[idx];

                    seq_t g1 = seq_gcd(f_num, den_k);
                    f_num /= g1;
                    den_k  /= g1;

                    seq_t g2 = seq_gcd(f_den, num_k);
                    f_den /= g2;
                    num_k  /= g2;

                    seq_t new_num = num_k * f_num;
                    seq_t new_den = den_k * f_den;
                    seq_t g3 = seq_gcd(new_num, new_den);
                    num_k = new_num / g3;
                    den_k = new_den / g3;
                }
            }
        }

        /* final reduction to be safe */
        seq_t gfinal = seq_gcd(num_k, den_k);
        num_k /= gfinal;
        den_k /= gfinal;

        num[k] = num_k;
        den[k] = den_k;
    }

    max_computed = n;
    return num[n];
}
