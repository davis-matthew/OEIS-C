/*
 * A330046  Expansion of e.g.f. exp(x) / (1 - sinh(x)).
 * https://oeis.org/A330046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * A006154(k), with A006154(0)=1
 *            and A006154(k)=φ(k) for k>0. Recurrence: a(0)=1; a(n)=1+Σ_{m
 *            odd, 1≤m≤n} binomial(n,m) * a(n-m)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;   /* array of computed a(n) values           */
static idx_t  a_cache_size = 0; /* number of slots allocated (size)       */

/* Multiplicative computation of a(n) using the recurrence above.
   a_cache is expanded as needed and all already-computed values are reused. */
seq_t a(idx_t n)
{
    /* Expand cache if required */
    if (a_cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* memory error, return 0 */
        a_cache = tmp;
        /* Compute a(k) for all k that are now missing (from old size up to n) */
        for (idx_t k = a_cache_size; k <= n; ++k) {
            seq_t sum = (seq_t)1;          /* start with the constant 1 for m=0 */
            seq_t binom = (seq_t)1;        /* binomial(k,0) = 1 */

            for (idx_t m = 1; m <= k; ++m) {
                /* binom = binom * (k - m + 1) / m  (exact division) */
                binom = binom * (seq_t)(k - m + 1) / (seq_t)m;
                if (m & 1) {              /* only odd m contribute */
                    sum += binom * a_cache[k - m];
                }
            }
            a_cache[k] = sum;
        }
        a_cache_size = new_size;
    }
    return a_cache[n];
}
