/*
 * A291733  a(n) = (1/4)*A291732(n).
 * https://oeis.org/A291733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 4*a(n-3) - 8*a(n-4) - 4*a(n-6)
 *            (n>=7) Initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=26, a(4)=72,
 *            a(5)=195, a(6)=520
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* dynamic cache for computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* allocated capacity */
static size_t computed_max = 0;  /* largest index for which a value has been computed */

/* enlarge the cache to store at least n+1 elements */
static void ensure_capacity(size_t n)
{
    size_t needed = n + 1;
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;                 /* double until big enough */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp)                          /* allocation failure */
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise cache with known initial values */
static void init_cache(void)
{
    if (cache == NULL)
        ensure_capacity(6);            /* need indices 0..6 */

    if (computed_max < 6) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)72;
        cache[5] = (seq_t)195;
        cache[6] = (seq_t)520;
        computed_max = 6;
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    init_cache();
    ensure_capacity(idx);

    /* compute missing values up to idx */
    while (computed_max < idx) {
        size_t k = computed_max + 1;   /* next index to compute */

        /* recurrence applies for k >= 7 (indices k-1 .. k-6 are already known) */
        seq_t val = ((seq_t)4) * cache[k - 1]
                  - ((seq_t)4) * cache[k - 2]
                  + ((seq_t)4) * cache[k - 3]
                  - ((seq_t)8) * cache[k - 4]
                  - ((seq_t)4) * cache[k - 6];

        cache[k] = val;
        computed_max = k;
    }

    return cache[idx];
}
