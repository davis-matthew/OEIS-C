/*
 * A303882  Number of n X 2 0..1 arrays with every element unequal to 0, 2, 3
 *          or 4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=8, a(4)=24, a(5)=82, a(6)=272,
 *            a(n)=4*a(n‑1)−2*a(n‑2)+2*a(n‑3)−6*a(n‑4)−4*a(n‑5) for n>6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of already computed terms.  cache[0] is unused.  */
static seq_t *cache = NULL;          /* cache[0] unused, cache[1..] holds a(1)..  */
static size_t cache_cap = 0;         /* allocated capacity (elements)           */
static idx_t  computed_up_to = 0;    /* highest n whose value is in cache       */

/* Ensure cache has space for indices up to n (inclusive).  Also
   initialise the first six terms on the first call.  */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)(n + 1U);       /* indices 0..n inclusive          */
    if (required < 7U)                       /* need space for a(1)..a(6)       */
        required = 7U;

    if (!cache || cache_cap < required) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failed               */
        cache = tmp;
        cache_cap = required;
    }

    /* Initialise first six terms only once.  */
    if (computed_up_to == 0) {
        cache[0] = 0;          /* unused */
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 8;
        cache[4] = 24;
        cache[5] = 82;
        cache[6] = 272;
        computed_up_to = 6;
    }
}

/* Return the n-th term of OEIS A303882 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* behaviour below offset is irrelevant */

    ensure_cache(n);

    if ((idx_t)(n) <= computed_up_to)
        return cache[(size_t)n];

    /* Compute successive terms up to n.  */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t term =
            4u * cache[(size_t)(i - 1)]
          - 2u * cache[(size_t)(i - 2)]
          + 2u * cache[(size_t)(i - 3)]
          - 6u * cache[(size_t)(i - 4)]
          - 4u * cache[(size_t)(i - 5)];
        cache[(size_t)i] = term;
    }
    computed_up_to = n;
    return cache[(size_t)n];
}
