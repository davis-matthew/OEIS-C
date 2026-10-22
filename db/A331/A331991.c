/*
 * A331991  Number of semi-lone-child-avoiding achiral rooted trees with n
 *          vertices.
 * https://oeis.org/A331991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=1; for n>1, a(n+1)=∑_{d|n, d<n} a(d) */

#include <stdlib.h>
#include <stdint.h>

/* static state -------------------------------------------------------- */
static seq_t *cache        = NULL;   /* cache[i] = a(i+1) */
static idx_t  cache_cap    = 0;      /* allocated size (elements) */
static idx_t  last_computed= 0;      /* largest n for which a(n) is known */

/* ensure cache can hold at least up to index n-1 */
static void ensure_cache(idx_t n) {
    if (n <= cache_cap) return;               /* already enough space */
    /* reallocate to the required size, using size_t for malloc */
    size_t new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                  /* unrecoverable error */
    cache       = new_cache;
    cache_cap   = (idx_t)new_cap;
}

/* main function -------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;                      /* undefined for n<1 */

    ensure_cache(n);                          /* guarantee storage */

    /* initialise the first two terms once */
    if (last_computed < 2) {
        /* make sure capacity is at least 2 before writing */
        ensure_cache(2);
        cache[0] = 1;
        cache[1] = 1;
        last_computed = 2;
    }

    /* compute terms from (last_computed+1) up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t target = i - 1;                 /* sum over d | (i-1), d<target */
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= target; ++d) {
            if (target % d == 0) {
                if (d < target)            /* proper divisor */
                    sum += cache[d - 1];
                idx_t other = target / d;
                if (other != d && other < target)
                    sum += cache[other - 1];
            }
        }
        cache[i - 1] = sum;
    }
    last_computed = n;

    return cache[n - 1];
}
