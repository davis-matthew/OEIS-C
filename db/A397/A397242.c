/*
 * A397242  O.g.f. satisfies A(x) = exp( x + Sum_{n>=2} (n^2-1) * a(n)*x^n /
 *          n^2 ).
 * https://oeis.org/A397242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(n) = n*a(n-1) + n * Σ_{k=1}^{n-2}
 *            k*(k+2)/(k+1) * a(k+1) * a(n-k-1) for n ≥ 2 This implements the
 *            recurrence (6) from the OEIS entry
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed terms */
static idx_t  cache_size   = 0;   /* allocated size of seq_cache */
static idx_t  computed_max = 0;   /* largest n that has been computed */

seq_t a(idx_t n)
{
    /* Allocate and initialise cache on first call */
    if (seq_cache == NULL) {
        cache_size = (n > 1 ? n + 1 : 2);
        seq_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        computed_max = 1;
    } else if (cache_size <= n) {
        size_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        seq_cache = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Return already computed value */
    if (n <= computed_max) return seq_cache[n];

    /* Compute terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = seq_cache[i - 1];               /* a(i-1) part */
        for (idx_t k = 1; k <= i - 2; ++k) {
            /* term = k*(k+2)/(k+1) * a(k+1) * a(i-k-1) */
            seq_t term = seq_cache[k + 1];
            term *= seq_cache[i - k - 1];
            term *= (seq_t)k;
            term *= ((seq_t)k + 2);
            term /= ((seq_t)k + 1);                  /* division is exact */
            sum += term;
        }
        seq_cache[i] = ((seq_t)i) * sum;              /* a(i) = i * sum */
    }
    computed_max = n;
    return seq_cache[n];
}
