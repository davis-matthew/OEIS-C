/*
 * A269012  Number of 2 X n binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 6*a(n-3) - 9*a(n-4) with a(1)=0,
 *            a(2)=4, a(3)=8, a(4)=36
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration of the function that will be called by the tests */
seq_t a(idx_t n);

static seq_t *cache = NULL;          /* array of computed terms, 1‑based */
static size_t cache_cap = 0;         /* maximal allocated index */
static idx_t cache_size = 0;         /* last index for which a value exists */

/* Ensure that the cache can hold up to index `max_needed` (inclusive) */
static void grow_cache(idx_t max_needed)
{
    if (max_needed <= cache_cap)
        return;        /* already large enough */

    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < (size_t)(max_needed + 1))
        new_cap <<= 1;          /* grow exponentially for amortised O(1) */

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the static cache the first time the function is called */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* Allocate space for indices 0…4 (index 0 unused) */
    cache_cap = 8;
    cache = (seq_t *)malloc(sizeof(seq_t) * (cache_cap + 1));
    /* The values are known from the OEIS entry */
    cache[0] = (seq_t)0;      /* unused but defined */
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)8;
    cache[4] = (seq_t)36;
    cache_size = 4;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for offset 1, but return 0 */
        return (seq_t)0;

    init_cache();

    if (n <= cache_size)
    {
        return cache[n];
    }

    grow_cache(n);

    /* Compute all missing terms up to n using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t term1 = cache[i - 1] * (seq_t)2;
        seq_t term2 = cache[i - 2] * (seq_t)5;
        seq_t term3 = cache[i - 3] * (seq_t)6;
        seq_t term4 = cache[i - 4] * (seq_t)9;

        seq_t sum = term1 + term2;
        seq_t sub = term3 + term4;

        cache[i] = sum - sub;        /* safe: sum ≥ sub for this sequence */
    }
    cache_size = n;
    return cache[n];
}
