/*
 * A354136  Expansion of e.g.f. exp(log(1 + x)^3/6).
 * https://oeis.org/A354136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} binomial(n-1, k-1) * Stirling1(k,3)
 *            * a(n-k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   binomial, ensure_enough
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ensure_enough.h"

/* cache for a(n) values */
static seq_t *a_cache   = NULL;
static size_t a_cap     = 0;
static idx_t  a_last    = 0;      /* highest n computed */

/* cache for Stirling1(k,3) values */
static seq_t *s3_cache  = NULL;
static size_t s3_cap    = 0;
static idx_t  s3_last   = 0;      /* highest k computed */

seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (!a_cache) {
        ensure_enough(&a_cache, &a_cap, 1);
        a_cache[0] = (seq_t)1;
        a_last = 0;
    }

    if (!s3_cache) {
        ensure_enough(&s3_cache, &s3_cap, 1);
        s3_cache[0] = (seq_t)0;   /* unused for k=0 */
        s3_last = 0;
        /* initialise Stirling numbers of the first kind for k=1 */
        seq_t prev_s1 = (seq_t)0;   /* s(0,1) */
        seq_t prev_s2 = (seq_t)0;   /* s(0,2) */
        seq_t prev_s3 = (seq_t)0;   /* s(0,3) */
        seq_t fact   = (seq_t)1;    /* (0)! */
        seq_t signv  = (seq_t)1;    /* (-1)^0 */
        for (idx_t t = 1; t <= n; ++t) {
            seq_t t_minus1 = (seq_t)(t - 1);
            seq_t curr_s1 = signv * fact;
            seq_t curr_s2 = prev_s1 - t_minus1 * prev_s2;
            seq_t curr_s3 = prev_s2 - t_minus1 * prev_s3;

            s3_cache[t] = curr_s3;

            prev_s1 = curr_s1;
            prev_s2 = curr_s2;
            prev_s3 = curr_s3;
            fact    = fact * t;          /* t!   */
            signv  = -signv;             /* toggle sign for next step */
        }
        s3_last = n;
    }

    /* compute Stirling1(k,3) for missing k up to n */
    if (n > s3_last) {
        ensure_enough(&s3_cache, &s3_cap, (size_t)n + 1);
        /* initialise recurrence state for next k = s3_last+1 */
        seq_t prev_s1 = (seq_t)0;      /* to be recomputed quickly */
        seq_t prev_s2 = (seq_t)0;
        seq_t prev_s3 = (seq_t)0;
        seq_t fact   = (seq_t)1;
        seq_t signv  = (seq_t)1;
        /* recompute states up to s3_last */
        for (idx_t t = 1; t <= s3_last; ++t) {
            seq_t t_minus1 = (seq_t)(t - 1);
            seq_t curr_s1 = signv * fact;
            seq_t curr_s2 = prev_s1 - t_minus1 * prev_s2;
            seq_t curr_s3 = prev_s2 - t_minus1 * prev_s3;

            prev_s1 = curr_s1;
            prev_s2 = curr_s2;
            prev_s3 = curr_s3;
            fact    = fact * t;
            signv  = -signv;
        }
        for (idx_t t = s3_last + 1; t <= n; ++t) {
            seq_t t_minus1 = (seq_t)(t - 1);
            seq_t curr_s1 = signv * fact;
            seq_t curr_s2 = prev_s1 - t_minus1 * prev_s2;
            seq_t curr_s3 = prev_s2 - t_minus1 * prev_s3;

            s3_cache[t] = curr_s3;

            prev_s1 = curr_s1;
            prev_s2 = curr_s2;
            prev_s3 = curr_s3;
            fact    = fact * t;
            signv  = -signv;
        }
        s3_last = n;
    }

    /* compute a(k) up to n */
    if (n > a_last) {
        ensure_enough(&a_cache, &a_cap, (size_t)n + 1);
        for (idx_t k = a_last + 1; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            idx_t k_minus1 = k - 1;
            for (idx_t t = 1; t <= k; ++t) {
                seq_t bin = binomial(k_minus1, t - 1);
                seq_t term = bin * s3_cache[t] * a_cache[k - t];
                sum += term;
            }
            a_cache[k] = sum;
        }
        a_last = n;
    }

    return a_cache[n];
}
