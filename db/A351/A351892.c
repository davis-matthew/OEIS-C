/*
 * A351892  Expansion of e.g.f. exp( sinh(sqrt(3)*x) / sqrt(3) ).
 * https://oeis.org/A351892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0 ; a(n) = Σ_{k=0..⌊(n−1)/2⌋} binomial(n−1,2k)
 *            * 3^k * a(n−2k−1) for n ≥ 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;          /* cached a(n) values */
static size_t cache_cap = 0;         /* allocated size of cache array */
static size_t computed = 0;          /* number of values already computed (cache[0..computed-1] valid) */

seq_t a(idx_t n)
{
    /* Ensure cache can hold a(n) */
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        cache_cap = need;
    } else if (need > cache_cap) {
        cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
        cache_cap = need;
    }

    /* initialize base case if not already done */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* compute missing values up to n (if any) */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        seq_t acc = (seq_t)0;
        idx_t limit = (idx_t)((i - 1) / 2);
        seq_t powk = (seq_t)1;                     /* 3^k, starting at k=0 */
        for (idx_t k = 0; k <= limit; ++k) {
            seq_t bin = binomial((idx_t)i - 1, (idx_t)2 * k);
            seq_t term = bin * powk * cache[i - 2 * k - 1];
            acc += term;
            powk *= 3ULL;                          /* next power of 3 */
        }
        cache[i] = acc;
    }
    computed = (size_t)n + 1;
    return cache[n];
}
