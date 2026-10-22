/*
 * A344935  a(0)=1; for n > 0, a(n) = n*(a(n-1) + i^(n-1)) if n is odd,
 *          n*a(n-1) + i^n otherwise, where i = sqrt(-1).
 * https://oeis.org/A344935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, if n is odd a(n) = n*(a(n-1) + i^(n-1)) if
 *            n is even a(n) = n*a(n-1) + i^n where i^k = 1, i, -1, -i
 *            repeating every 4, and the sequence values are real integers.
 *            Implementation uses integer arithmetic only, precomputing and
 *            caching values
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static idx_t cached_up_to = 0;   /* highest index stored in cache */

/* Compute and cache terms up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate at least one slot for a(0) */
        size_t sz = (n + 1 > 1) ? (size_t)(n + 1) : 1;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cached_up_to = 0;
    } else if (cached_up_to < n) {
        /* grow cache if necessary */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;
            if (i & 1) {  /* n is odd */
                if (((i - 1) >> 1) & 1) {
                    /* i^(n-1) = -1 */
                    cache[i] = i_seq * (cache[i - 1] - (seq_t)1);
                } else {
                    /* i^(n-1) = +1 */
                    cache[i] = i_seq * (cache[i - 1] + (seq_t)1);
                }
            } else {      /* n is even */
                if ((i >> 1) & 1) {
                    /* i^n = -1 */
                    cache[i] = i_seq * cache[i - 1] - (seq_t)1;
                } else {
                    /* i^n = +1 */
                    cache[i] = i_seq * cache[i - 1] + (seq_t)1;
                }
            }
        }
        cached_up_to = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
