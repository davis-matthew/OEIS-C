/*
 * A291222  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S^2 - S^3.
 * https://oeis.org/A291222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-3) - 4*a(n-4) + a(n-6) for n>=7; a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=5, a(5)=9, a(6)=19
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached computed values */
static idx_t   cache_size = 0;       /* number of elements cached (>= 1) */

/* base values a(0) .. a(6) */
static const seq_t base_vals[7] = {0, 1, 1, 3, 5, 9, 19};

/* Ensure that cache contains values up to index n.  New entries are
   computed using the recurrence.  All arithmetic is performed in
   seq_t, which is the required integer type. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;               /* already good */

    idx_t new_size = (n + 1 > 7 ? n + 1 : 7);  /* at least up to base */

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);       /* out of memory */

    cache = new_cache;
    if (cache_size == 0) {
        /* first time: set base values */
        for (idx_t i = 0; i < 7 && i < new_size; ++i)
            cache[i] = base_vals[i];
    }

    /* compute any newly required entries */
    idx_t start = cache_size;
    if (start < 7) start = 7;                 /* base values already set */

    for (idx_t i = start; i <= n; ++i) {
        seq_t sum1 = ((seq_t)4 * cache[i - 2]) + cache[i - 3] + cache[i - 6];
        cache[i]   = sum1 - ((seq_t)4 * cache[i - 4]);
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
