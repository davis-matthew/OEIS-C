/*
 * A367425  Expansion of e.g.f. 1 / (1 + log(1 - 3*x))^(2/3).
 * https://oeis.org/A367425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} 3^k * (3n - k) / (3n) * (k-1)! * binomial(n,k)
 *            * a(n-k) with a(0) = 1. This implementation uses memoization and
 *            cancels the common divisor 3*n factor for each summand to avoid
 *            overflow on 64‑bit builds while still working with
 *            arbitrary‑precision seq_t
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (idx_t idx = cache_size; idx <= n; ++idx) {
            if (idx == 0) {
                cache[0] = (seq_t)1;
                continue;
            }

            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* C(idx,0) */
            seq_t fact  = (seq_t)1;          /* (k-1)! for k=1 */
            seq_t pow3  = (seq_t)3;          /* 3^k for k=1 */

            for (idx_t k = 1; k <= idx; ++k) {

                /* Update binomial coefficient C(idx,k) */
                binom = binom * ((seq_t)(idx - k + 1)) / (seq_t)k;

                /* Prepare components of the summand */
                seq_t comp_pow3   = pow3;
                seq_t comp_3n_minus_k = (seq_t)(3 * idx - k);
                seq_t comp_fact   = fact;
                seq_t comp_binom  = binom;
                seq_t comp_a      = cache[idx - k];

                /* Denominator 3*idx */
                seq_t denom = (seq_t)3 * (seq_t)idx;

                /* Compute the term, cancelling common factors with denom */
                seq_t term = (seq_t)1;

                /* Helper for each component */
                seq_t tmp;

                /* pow3 component */
                tmp = comp_pow3;
                if (denom > (seq_t)1) {
                    seq_t g = gcd(tmp, denom);
                    if (g > (seq_t)1) { tmp /= g; denom /= g; }
                }
                term *= tmp;

                /* (3n - k) component */
                tmp = comp_3n_minus_k;
                if (denom > (seq_t)1) {
                    seq_t g = gcd(tmp, denom);
                    if (g > (seq_t)1) { tmp /= g; denom /= g; }
                }
                term *= tmp;

                /* (k-1)! component */
                tmp = comp_fact;
                if (denom > (seq_t)1) {
                    seq_t g = gcd(tmp, denom);
                    if (g > (seq_t)1) { tmp /= g; denom /= g; }
                }
                term *= tmp;

                /* binomial component */
                tmp = comp_binom;
                if (denom > (seq_t)1) {
                    seq_t g = gcd(tmp, denom);
                    if (g > (seq_t)1) { tmp /= g; denom /= g; }
                }
                term *= tmp;

                /* a(idx-k) component */
                tmp = comp_a;
                if (denom > (seq_t)1) {
                    seq_t g = gcd(tmp, denom);
                    if (g > (seq_t)1) { tmp /= g; denom /= g; }
                }
                term *= tmp;

                /* Any remaining divisor must divide term exactly */
                if (denom > (seq_t)1)
                    term /= denom;

                sum += term;

                /* Prepare next iterates */
                fact *= (seq_t)k;   /* now fact = k! for next k (which will be (k+1)-1)! */
                pow3 *= (seq_t)3;   /* 3^(k+1) */
            }

            cache[idx] = sum;
        }

        cache_size = (idx_t)new_size;
    }

    return cache[n];
}
