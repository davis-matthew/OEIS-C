/*
 * A242587  The number of conjugacy classes of n X n matrices over F_3.
 * https://oeis.org/A242587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/Product_{r>=1} (1-3*x^r)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache = NULL;   /* array a[0..current_max] */
static idx_t current_max = 0; /* highest index currently computed */

/* Full recomputation of the sequence up to N, overwriting cache */
static void compute_full(idx_t N)
{
    /* Allocate or resize cache */
    seq_t *new_cache = (seq_t *)realloc(cache, (N + 1) * sizeof(seq_t));
    if (!new_cache) abort();   /* allocation failure */
    cache = new_cache;

    /* Initialise to zero and set a(0)=1 */
    for (idx_t i = 0; i <= N; ++i) cache[i] = (seq_t)0;
    cache[0] = (seq_t)1;

    /* Standard partition‑style DP with weight 3 for each part */
    for (idx_t r = 1; r <= N; ++r)
    {
        for (idx_t s = r; s <= N; ++s)
        {
            cache[s] += cache[s - r] * ((seq_t)3);
        }
    }

    current_max = N;
}

seq_t a(idx_t n)
{
    /* Lazily compute up to the required index */
    if (!cache || n > current_max)
        compute_full(n);

    return cache[n];
}
