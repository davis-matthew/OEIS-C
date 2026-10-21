/*
 * A254758  Part of the positive proper solutions x of the Pell equation x^2 -
 *          2*y^2 = - 7^2 based on the fundamental solution (x0, y0)= (1, 5).
 * https://oeis.org/A254758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2); a(0)=1, a(1)=23
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for computed terms */
static seq_t *cache   = NULL;   /* array of terms */
static size_t cache_cap = 0;    /* current capacity of the array */
static idx_t max_computed = 0;  /* highest index computed so far */

/* Ensure the array can store indices 0 .. need-1 */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 23;

    /* Initialise cache on first use */
    if (!cache) {
        ensure_capacity(2);
        cache[0] = 1;
        cache[1] = 23;
        max_computed = 1;
    }

    /* Make sure we can address index n */
    ensure_capacity((size_t)n + 1);

    /* Compute missing terms up to n */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t tmp = ((seq_t)6) * cache[i-1];
            cache[i] = tmp - cache[i-2];
        }
        max_computed = n;
    }

    return cache[n];
}
