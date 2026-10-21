/*
 * A271919  Numerator of Product_{j=1..n-1} ((3*j+1)/(3*j+2)).
 * https://oeis.org/A271919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ∏_{j=1}^{n‑1} ((3j+1)/(3j+2))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(1) = 1, a(2) = 4, a(3) = 7, …                    */
/* The sequence is the numerator of the running product of fractions.   */

#include <stdlib.h>
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* Cache of numerators and denominators for 1..max_cached */
    static seq_t *num_cache = NULL;
    static seq_t *den_cache = NULL;
    static idx_t max_cached = 0;

    if (n == 0)                     /* OEIS is 1‑based; 0 is undefined */
        return (seq_t)0;

    /* Resize cache if necessary */
    if (n > max_cached) {
        size_t new_cap = (size_t)(n + 1);          /* indices 0..n            */
        seq_t *tmp_num = (seq_t *)realloc(num_cache,
                                         new_cap * sizeof(seq_t));
        seq_t *tmp_den = (seq_t *)realloc(den_cache,
                                         new_cap * sizeof(seq_t));
        if (!tmp_num || !tmp_den) {
            /* Allocation failed – abort */
            abort();
        }
        num_cache = tmp_num;
        den_cache = tmp_den;

        /* Initialise base case a(1)=1/1 if not yet done */
        if (max_cached == 0) {
            num_cache[1] = (seq_t)1;
            den_cache[1] = (seq_t)1;
            max_cached  = 1;
        }

        /* Compute successive terms up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            idx_t j = i - 1;                       /* current product index */

            /* New fraction factor (3j+1)/(3j+2) */
            seq_t f_num = (seq_t)(3 * j + 1);      /* numerator part */
            seq_t f_den = (seq_t)(3 * j + 2);      /* denominator part */

            /* Previous fraction */
            seq_t p_num = num_cache[i - 1];
            seq_t p_den = den_cache[i - 1];

            /* Cross‑reduce to keep intermediate values small */
            seq_t g1 = gcd(p_num, f_den);
            if (g1 > (seq_t)1) {
                p_num = p_num / g1;
                f_den = f_den / g1;
            }

            seq_t g2 = gcd(p_den, f_num);
            if (g2 > (seq_t)1) {
                p_den = p_den / g2;
                f_num = f_num / g2;
            }

            /* Multiply the reduced fractions */
            num_cache[i] = seq_mul(p_num, f_num);
            den_cache[i] = seq_mul(p_den, f_den);
        }

        max_cached = n;
    }

    return num_cache[n];
}
