/*
 * A381345  Expansion of e.g.f. 1/( 1 - x * cos(sqrt(2)*x) ).
 * https://oeis.org/A381345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-2)^k * (2*k+1) *
 *            binomial(n,2*k+1) * a(n-2*k-1), a(0)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_max = 0;

/* ensure cache has capacity for indices up to n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    cache = new_cache;
    cache_size = n + 1;
}

/* initialise the cache for a(0) */
static void init_cache(void)
{
    if (cache == NULL) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        cache[0] = 1;          /* a(0) = 1 */
        cache_size = 1;
        computed_max = 0;
    }
}

seq_t a(idx_t n)
{
    /* initialise for the first call */
    if (cache == NULL)
        init_cache();

    /* extend cache if needed */
    if (n >= cache_size)
        ensure_cache(n);

    /* compute missing terms up to n */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t maxk = (i - 1) / 2;
            seq_t pow_neg2 = 1;             /* (-2)^k */
            idx_t m = 1;                   /* current 2*k+1 */
            seq_t bin = (seq_t)i;          /* binomial(i,1) = i */

            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t term = pow_neg2 * (seq_t)m * bin * cache[i - m];
                sum += term;
                pow_neg2 *= (seq_t)(-2);   /* prepare next power of -2 */

                if (k < maxk) {
                    /* update bin for next m = m + 2:
                     * bin(i, m+2) = bin(i, m) * (i-m)*(i-m-1) / [(m+1)*(m+2)]   */
                    seq_t num1 = (seq_t)(i - m);
                    seq_t num2 = (seq_t)(i - m - 1);
                    seq_t den1 = (seq_t)(m + 1);
                    seq_t den2 = (seq_t)(m + 2);

                    seq_t numerator   = num1 * num2;
                    seq_t denominator = den1 * den2;

                    bin = (bin * numerator) / denominator;
                    m += 2;
                }
            }
            cache[i] = sum;
        }
        computed_max = n;
    }
    return cache[n];
}
