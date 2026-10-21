/*
 * A251601  Numbers k such that hexagonal numbers H(k) and H(k+1) sum to
 *          another hexagonal number.
 * https://oeis.org/A251601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251601: 
   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3),   n >= 4
   a(1) = 0, a(2) = 13, a(3) = 450
   Offset = 1
*/

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* array holding the sequence values */
static idx_t  cache_size = 0;    /* total allocated slots */
static idx_t  computed_len = 0;  /* highest index computed (1‑based) */

/* ensure that there is space for at least n elements */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_size)
        return;
    size_t new_size = cache_size ? cache_size << 1 : 4;
    while (new_size < (size_t)n)
        new_size <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, computed_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_size = (idx_t)new_size;
}

/* compute terms up to n if needed */
static void compute_up_to(idx_t n)
{
    if (n <= computed_len)
        return;
    ensure_capacity(n);
    for (idx_t i = computed_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[0] = (seq_t)0;
        else if (i == 2)
            cache[1] = (seq_t)13;
        else if (i == 3)
            cache[2] = (seq_t)450;
        else
            cache[i - 1] = 35 * cache[i - 2] - 35 * cache[i - 3] + cache[i - 4];
    }
    computed_len = n;
}

/* public entry point required by the evaluator */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1, so this case is undefined; return 0 */
        return (seq_t)0;
    compute_up_to(n);
    return cache[n - 1];
}
