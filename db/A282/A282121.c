/*
 * A282121  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 430", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*a(n-2) - 100*a(n-4) for n > 3 Initial values: a(0) =
 *            1, a(1) = 11, a(2) = 110, a(3) = 1111
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* dynamic array of computed terms */
static size_t  cap    = 0;      /* current allocated capacity */
static idx_t   last   = (idx_t)-1;  /* highest index that has been computed */

/* ensure the cache can hold at least indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation: at least 4 entries for the known terms */
        size_t init = (n < 4) ? 4 : n + 1;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cap   = init;
        /* set the initial four terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)110;
        cache[3] = (seq_t)1111;
        last = 3;
    } else if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cap = new_cap;
    }
}

/* compute terms up to n, if necessary */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);
    /* already have all values up to last */
    if (n <= last) return;

    for (idx_t i = last + 1; (idx_t)i <= n; ++i) {
        if (i <= 3) {
            /* indices 0..3 already initialized */
            continue;
        }
        /* recurrence: a(i) = 101*a(i-2) - 100*a(i-4) */
        seq_t term1 = 101 * cache[i-2];
        seq_t term2 = 100 * cache[i-4];
        cache[i] = term1 - term2;   /* valid: term1 >= term2 for all i */
    }
    last = n;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
