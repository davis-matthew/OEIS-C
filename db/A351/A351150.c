/*
 * A351150  G.f. A(x) satisfies: A(x) = 1 + x^2 * A(x/(1 - 4*x)) / (1 - 4*x).
 * https://oeis.org/A351150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-2} binomial(n-2,k) * 4^k * a(n-k-2) with a(0)
 *            = 1, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   ensure_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_to.h"

/* Dynamically allocated cache for the sequence values */
static seq_t *a_cache  = NULL;
static size_t a_cap    = 0;
static idx_t a_len     = 0;   /* highest index computed (inclusive) */

/* Dynamically allocated cache for powers of 4 */
static seq_t *pow4_cache = NULL;
static size_t pow4_cap   = 0;
static idx_t pow4_len    = 0;  /* highest power computed (inclusive) */

/* Main function computing the sequence */
seq_t a(idx_t n)
{
    /* Initialise base cases and caches if they have not been used yet */
    if (a_cache == NULL)
    {
        ensure_to(1, &a_cache, &a_cap);     /* need indices 0 and 1 */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)0;
        a_len = 1;
    }

    if (pow4_cache == NULL)
    {
        ensure_to(0, &pow4_cache, &pow4_cap);  /* index 0 */
        pow4_cache[0] = (seq_t)1;
        pow4_len = 0;
    }

    /* If requested index is already computed, just return it */
    if (n <= a_len)
        return a_cache[n];

    /* Make sure we have pre‑computed enough powers of 4 for the largest L = n‑2 */
    idx_t max_L = (n >= 2) ? n - 2 : 0;
    if (pow4_len < max_L)
    {
        ensure_to(max_L, &pow4_cache, &pow4_cap);
        for (idx_t i = pow4_len + 1; i <= max_L; ++i)
            pow4_cache[i] = pow4_cache[i - 1] * (seq_t)4;
        pow4_len = max_L;
    }

    /* Compute missing terms up to 'n' */
    for (idx_t m = a_len + 1; m <= n; ++m)
    {
        idx_t L = (m >= 2) ? m - 2 : 0;          /* L = m‑2 (>=0) */
        seq_t sum = 0;
        seq_t binom = (seq_t)1;                 /* C(L,0) */

        for (idx_t j = 0; j <= L; ++j)
        {
            /* term: a(j) * binom * 4^(L-j) */
            seq_t term = a_cache[j] * binom;
            term *= pow4_cache[L - j];
            sum += term;

            /* update binom for next j */
            if (j < L)
            {
                seq_t num = (seq_t)(L - j);
                seq_t den = (seq_t)(j + 1);
                binom = binom * num / den;     /* exact division */
            }
        }

        /* Append the newly computed value to the cache */
        if (m >= a_cap)
            ensure_to(m, &a_cache, &a_cap);
        a_cache[m] = sum;
    }

    a_len = n;
    return a_cache[n];
}
