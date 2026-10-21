/*
 * A297067  Number of n X 2 0..1 arrays with no 1 adjacent to 1 king-move
 *          neighboring 1.
 * https://oeis.org/A297067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) – a(n-2) – a(n-3) – 4*a(n-4) – 8*a(n-5) Initial
 *            values: a(1)=3, a(2)=10, a(3)=32, a(4)=103, a(5)=350
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* Memoisation array */
static size_t cache_cap = 0;         /* Allocated capacity (in elements) */
static size_t cache_len = 0;         /* Number of computed entries */

/* Ensure the memoisation array has capacity for at least `min_cap` entries. */
static void ensure_cap(size_t min_cap)
{
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* Out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* Offset starts at 1 */

    /* The recurrence needs five base terms. */
    ensure_cap(5);

    /* Grow the cache if necessary for the requested index. */
    if ((size_t)n > cache_cap) ensure_cap((size_t)n);

    /* Initialise the base terms on first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;    /* a(1) */
        cache[1] = (seq_t)10;   /* a(2) */
        cache[2] = (seq_t)32;   /* a(3) */
        cache[3] = (seq_t)103;  /* a(4) */
        cache[4] = (seq_t)350;  /* a(5) */
        cache_len = 5;
    }

    /* If already computed, return the value. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute successive terms up to n using the recurrence. */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t val =
            ((seq_t)4 * cache[i - 2])              /* 4*a(i‑1) */
            - cache[i - 3]                        /* – a(i‑2) */
            - cache[i - 4]                        /* – a(i‑3) */
            - ((seq_t)4 * cache[i - 5])           /* – 4*a(i‑4) */
            - ((seq_t)8 * cache[i - 6]);          /* – 8*a(i‑5) */
        cache[i - 1] = val;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
