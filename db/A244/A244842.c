/*
 * A244842  a(n) = (10^n - 1)*(10^n - 10)/90.
 * https://oeis.org/A244842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n - 1)*(10^n - 10)/90, n >= 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *pow10_cache = NULL;      /* cache[0] = 1, cache[i] = 10^i */
static idx_t pow10_cache_used = 0;     /* number of valid entries in cache */
static idx_t pow10_cache_cap  = 0;     /* allocated size of cache */

/* return 10^n, using a growing cache */
static seq_t pow10_cache_get(idx_t n)
{
    if (pow10_cache_used == 0) {               /* initialize cache[0] = 1 */
        pow10_cache_cap = 1;
        pow10_cache = (seq_t*)malloc(pow10_cache_cap * sizeof(seq_t));
        if (!pow10_cache) abort();
        pow10_cache[0] = 1;
        pow10_cache_used = 1;
    }
    if (n >= pow10_cache_used) {
        /* grow cache to at least n+1 entries */
        if (n + 1 > pow10_cache_cap) {
            idx_t new_cap = (n + 1) * 2;
            seq_t *tmp = (seq_t*)realloc(pow10_cache,
                                         new_cap * sizeof(seq_t));
            if (!tmp) abort();
            pow10_cache = tmp;
            pow10_cache_cap = new_cap;
        }
        /* compute missing powers */
        for (idx_t i = pow10_cache_used; i <= n; ++i) {
            pow10_cache[i] = pow10_cache[i-1] * 10;
        }
        pow10_cache_used = n + 1;
    }
    return pow10_cache[n];
}

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t pow10n = pow10_cache_get(n);

    /* (10^n - 1) and (10^n - 10) */
    seq_t t1 = pow10n - 1;
    seq_t t2 = pow10n - 10;

    /* Divide the factors to avoid overflow:
       10^n - 1 is divisible by 9, 10^n - 10 is divisible by 10. */
    seq_t a_part = t1 / 9;
    seq_t b_part = t2 / 10;

    /* final product */
    return a_part * b_part;
}
