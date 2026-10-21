/*
 * A270985  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 238", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (320 - 27·2^(3+n) + 175·4^n + 384·n) / 144 (for n>2)
   Recurrence: a(n) = 8·a(n-1) - 21·a(n-2) + 22·a(n-3) - 8·a(n-4) (for n>6)
   Initial values: a(0)=1, a(1)=5, a(2)=20, a(3)=76, a(4)=300,
                    a(5)=1212, a(6)=4900 */

#include <stdlib.h>

/* Cached values of the sequence.  The array is grown lazily. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of values stored (max index + 1)   */
static idx_t cache_cap = 0;   /* current allocated capacity */

/* Initial seven terms.  They are used for all n<7. */
static const seq_t init_vals[7] = {
    (seq_t)1, (seq_t)5, (seq_t)20, (seq_t)76,
    (seq_t)300, (seq_t)1212, (seq_t)4900
};

/* Ensure the cache holds room for indices up to 'need' (inclusive). */
static void ensure_cache(idx_t need)
{
    if (need < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : (idx_t)64;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache structure on first use. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap = 64;
    cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;
}

/* Extend the cache so that it contains a(0) … a(n). */
static void extend_to(idx_t n)
{
    if (n < cache_len)
        return;

    ensure_cache(n);

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i < 7)                          /* initial values */
        {
            cache[i] = init_vals[i];
            continue;
        }

        /* a(i) = 8*a(i-1) - 21*a(i-2) + 22*a(i-3) - 8*a(i-4) */
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];

        seq_t res = a1 * (seq_t)8;
        res -= a2 * (seq_t)21;
        res += a3 * (seq_t)22;
        res -= a4 * (seq_t)8;

        cache[i] = res;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    init_cache();
    extend_to(n);
    return cache[n];
}
