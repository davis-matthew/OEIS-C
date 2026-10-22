/*
 * A351056  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 - 5*x)) / (1 -
 *          5*x).
 * https://oeis.org/A351056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=1;
   a(n)=Σ_{k=0}^{n-2} binomial(n-2,k) * 5^k * a(n-k-2)   (n≥2) */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* computed values  */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  cache_max  = 0;      /* highest index already computed */

/* enlarge cache array to at least size n+1 */
static void ensure_cache_size(idx_t n)
{
    if (!cache || cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        if (cache) {
            for (size_t i = 0; i < old_cap; ++i) new_arr[i] = cache[i];
            free(cache);
        }
        for (size_t i = old_cap; i < new_cap; ++i) new_arr[i] = 0;
        cache      = new_arr;
        cache_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initial values */
    if (n == 0 || n == 1) return 1;

    /* allocate basic cache if not yet done */
    if (!cache) {
        ensure_cache_size(1);
        cache[0] = 1;
        cache[1] = 1;
        cache_max = 1;
    }

    if (n > cache_max) {
        ensure_cache_size(n);

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            idx_t m = i - 2;           /* n-2 for this term */
            seq_t sum   = 0;
            seq_t binom = 1;           /* C(m,0) */
            seq_t pow5  = 1;           /* 5^0 */

            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = binom * pow5 * cache[(size_t)(m - k)];
                sum += term;

                /* next binomial coefficient */
                if (k < m) {
                    seq_t num = binom * (seq_t)(m - k);
                    binom = num / (seq_t)(k + 1);
                }
                /* next power of 5 */
                if (k < m) pow5 *= (seq_t)5;
            }
            cache[(size_t)i] = sum;
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}
