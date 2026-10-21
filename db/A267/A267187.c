/*
 * A267187  Column 4 of the square array in A267181.
 * https://oeis.org/A267187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A267187 – Column 4 of A267181
    Recurrence: a(n) = a(n‑1) + a(n‑4) – a(n‑5)        for n ≥ 10
    Corrections for n = 6…9:  add n_term[n] where
        n_term = {0, 3, –2, 3, –4, 3, 0, 0, –1, –1}
    Initial values (OEIS offset 0):
        a(0)=0, a(1)=3, a(2)=1, a(3)=4, a(4)=0, a(5)=6
*/
#include <stdlib.h>   /* malloc, realloc, exit, NULL */

static seq_t *cache = NULL;        /* memoisation array */
static size_t cache_capacity = 0;  /* allocated size */
static idx_t  computed_max  = 5;   /* highest index pre‑computed */

/* Ensure that the cache can store values up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((idx_t)cache_capacity > n) return;

    size_t new_cap = cache_capacity ? cache_capacity : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    for (size_t i = cache_capacity; i < new_cap; ++i) cache[i] = 0;
    cache_capacity = new_cap;
}

/* Correction terms for n = 0 … 9  (integer constants). */
static const int n_term[10] = {0, 3, -2, 3, -4, 3, 0, 0, -1, -1};

/* Compute a(n) for any non‑negative index n.  */
seq_t a(idx_t n)
{
    /* Lazy initialisation: allocate and fill the base terms. */
    if (!cache) {
        ensure_capacity(n);
        cache[0] = 0;
        cache[1] = 3;
        cache[2] = 1;
        cache[3] = 4;
        cache[4] = 0;
        cache[5] = 6;
        computed_max = 5;
    }

    if (n < computed_max) return cache[n];
    if (n > computed_max) ensure_capacity(n);

    /* Extend the cache up to index n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 9) {
            /* Use the corrective term for 6 ≤ i ≤ 9. */
            seq_t val = cache[i-1] + cache[i-4];
            val -= cache[i-5];
            if (n_term[i] > 0) val += (seq_t)n_term[i];
            else if (n_term[i] < 0) val -= (seq_t)(-n_term[i]);
            cache[i] = val;
        } else {
            /* Recurrence for n ≥ 10. */
            cache[i] = cache[i-1] + cache[i-4] - cache[i-5];
        }
    }
    computed_max = n;
    return cache[n];
}
