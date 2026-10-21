/*
 * A268776  Number of n X 3 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two no more than once.
 * https://oeis.org/A268776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) + 2*a(n-2) - 16*a(n-3) - a(n-4) + 12*a(n-5) - 4*a(n-6)
   Initial values: a(1)=7, a(2)=26, a(3)=91, a(4)=316, a(5)=1031, a(6)=3354 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation array, cache[0] unused */
static idx_t  cache_cap = 0;     /* allocated capacity (in elements)      */
static idx_t  computed_max = 0;  /* highest n that has been computed      */

/* Ensure the cache can hold indices up to 'need' (inclusive) */
static void ensure_cache(idx_t need)
{
    if (cache_cap > need + 1) return;        /* +1 because indices start at 1 */
    idx_t new_cap = need + 16;               /* small growth factor            */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                       /* memory allocation failed       */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first six terms (valid for all n ≥ 1) */
static void init_base(void)
{
    cache[1] = (seq_t)7;
    cache[2] = (seq_t)26;
    cache[3] = (seq_t)91;
    cache[4] = (seq_t)316;
    cache[5] = (seq_t)1031;
    cache[6] = (seq_t)3354;
    computed_max = 6;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */

    /* Ensure we have at least the first six entries */
    if (!cache) ensure_cache(6);
    if (computed_max < 6) init_base();

    /* For indices ≤ 6 just return the stored value */
    if (n <= 6) return cache[n];

    /* Grow the cache if necessary */
    ensure_cache(n);

    /* Compute missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        seq_t pos = 4 * cache[i-1] + 2 * cache[i-2] + 12 * cache[i-5];
        seq_t neg = 16 * cache[i-3] +       cache[i-4] + 4 * cache[i-6];
        cache[i] = pos - neg;   /* pos >= neg for all i ≥ 7, hence no underflow */
    }

    computed_max = n;
    return cache[n];
}
