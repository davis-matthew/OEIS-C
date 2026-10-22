/*
 * A352119  Expansion of e.g.f. 1/(2 - exp(4*x))^(1/4).
 * https://oeis.org/A352119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^(n-k) * (Product_{j=0..k-1} (4*j+1)) *
 *            Stirling2(n,k) Recurrence used: a(0)=1, a(n) = a(n-1) - 2 *
 *            Σ_{k=1}^{n-1} (-4)^k * C(n-1,k) * a(n-k) (n≥1) This
 *            implementation avoids fractions and negative numbers by
 *            splitting the sum into positive and negative parts
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"   /* provides seq_realloc(seq_t*, size_t, size_t) */

static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Ensure the cache has room for indices up to LIMIT (inclusive). */
static void ensure_cache(size_t limit)
{
    if (cache_cap > limit)
        return;

    size_t new_cap = limit + 1;
    seq_t *new_buf = seq_realloc(seq_cache, cache_cap, new_cap);
    if (!new_buf)
        exit(1);  /* out of memory */
    seq_cache = new_buf;
    cache_cap = new_cap;
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    /* Start with a(0)=1. */
    if (cache_size == 0) {
        ensure_cache(0);
        seq_cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (n == 0)
        return seq_cache[0];

    /* Grow cache if necessary. */
    if ((size_t)n >= cache_cap)
        ensure_cache((size_t)n);

    /* Compute missing terms up to n. */
    for (size_t cur = cache_size; cur <= (size_t)n; ++cur) {
        if (cur == 0) {   /* already initialized */
            continue;
        }

        seq_t pos_sum = 0;   /* Σ of terms with even k (positive contributors) */
        seq_t neg_sum = 0;   /* Σ of terms with odd k  (negative contributors) */

        seq_t pow4 = 1;      /* will become 4^k in the loop */
        seq_t bin   = 1;      /* binomial(cur-1, k) initially for k=0 */

        for (idx_t k = 1; k < cur; ++k) {
            /* Update 4^k */
            pow4 *= (seq_t)4;

            /* Update binomial(cur-1, k) from binomial(cur-1, k-1) */
            if (k == 1) {
                bin = (seq_t)(cur - 1);
            } else {
                bin = bin * (seq_t)(cur - 1 - (k - 1));
                bin = bin / (seq_t)k;
            }

            /* term = 4^k * C(cur-1,k) * a[cur-k] */
            seq_t term = pow4 * bin;
            term = term * seq_cache[cur - k];

            if (k & 1)
                neg_sum += term;   /* odd k contributes negatively */
            else
                pos_sum += term;   /* even k contributes positively */
        }

        /* a[cur] = a[cur-1] - 2*(neg_sum - pos_sum) */
        seq_t diff;
        seq_t a_prev = seq_cache[cur - 1];
        if (neg_sum >= pos_sum) {
            diff = neg_sum - pos_sum;
            seq_cache[cur] = a_prev + (seq_t)2 * diff;
        } else {
            diff = pos_sum - neg_sum;
            seq_cache[cur] = a_prev - (seq_t)2 * diff;
        }
    }

    cache_size = (size_t)n + 1;
    return seq_cache[n];
}
