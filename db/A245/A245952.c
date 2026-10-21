/*
 * A245952  Number of length 2+3 0..n arrays with some pair in every
 *          consecutive four terms totalling exactly n.
 * https://oeis.org/A245952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 6*a(n-3) + 6*a(n-5) - 2*a(n-6) -
 *            2*a(n-7) + a(n-8), n > 8 Initial values: a(1)=26, a(2)=197,
 *            a(3)=676, a(4)=1889, a(5)=3966, a(6)=7669, a(7)=13064,
 *            a(8)=21281 This implements memoised iterative computation; works
 *            for any call order and n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 193, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* 1‑based values stored at index n-1 */
static idx_t  cache_len = 0;       /* current cache size */

seq_t a(idx_t n)
{
    /* Guard against invalid input; behaviour undefined for n==0 */
    if (n == 0) return 0;

    /* Ensure cache can hold up to n */
    if (cache_len < n) {
        /* Allocate or grow cache to size n */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failure: aborting */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;

        /* Initialise new entries */
        static const seq_t init[8] = {26, 197, 676, 1889, 3966, 7669, 13064, 21281};

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 8) {
                cache[i-1] = init[i-1];
            } else {
                /* Recurrence: a(i) = 2*a(i-1) + 2*a(i-2) - 6*a(i-3) + 6*a(i-5)
                               - 2*a(i-6) - 2*a(i-7) + a(i-8) */
                seq_t val =  2 * cache[i-2] +          /* a(i-1)   */
                              2 * cache[i-3] -          /* a(i-2)   */
                             6 * cache[i-4] +          /* a(i-3)   */
                             6 * cache[i-6] -          /* a(i-5)   */
                             2 * cache[i-7] -          /* a(i-6)   */
                             2 * cache[i-8] +          /* a(i-7)   */
                               cache[i-9];            /* a(i-8)   */
                cache[i-1] = val;
            }
        }
        cache_len = n;
    }

    return cache[n-1];
}
