/*
 * A279866  Number of n X 2 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) - 14*a(n-3) + 19*a(n-4) + 13*a(n-5) *
 *            - 22*a(n-6) - 4*a(n-7) + 8*a(n-8) for n > 9 * Initial values (n
 *            = 1 … 9): * a(1)=1, a(2)=4, a(3)=33, a(4)=96, a(5)=255, *
 *            a(6)=610, a(7)=1437, a(8)=3292, a(9)=7451
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

extern seq_t a(idx_t n);   /* forward declaration to suppress warnings */

/* ---------- Implementation ---------- */

static seq_t *cache      = NULL;   /* array a[0] … a[max]   */
static size_t cache_cap  = 0;     /* allocated size (elements)  */
static idx_t  cache_max  = 0;     /* highest index computed     */

static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;                 /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return;      /* allocation failure */
        /* dummy a(0) */
        cache[0] = (seq_t)0;
        /* fill known values a(1) … a(9) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)33;
        cache[4] = (seq_t)96;
        cache[5] = (seq_t)255;
        cache[6] = (seq_t)610;
        cache[7] = (seq_t)1437;
        cache[8] = (seq_t)3292;
        cache[9] = (seq_t)7451;
        cache_max = 9;
    }

    /* grow if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;   /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined; avoid UB */

    ensure_capacity(n);
    if (cache == NULL) return (seq_t)0;     /* allocation failure */

    /* Return already computed value */
    if (n <= cache_max) return cache[(size_t)n];

    /* Compute from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /*  i ≥ 10 because cache_max ≥ 9 at this point */
        seq_t val = 0;
        /* Apply recurrence in an order that keeps every intermediate
           non‑negative (see proof in the specification). */
        val += cache[(size_t)(i - 1)] * (seq_t)5;
        val -= cache[(size_t)(i - 2)] * (seq_t)4;
        val -= cache[(size_t)(i - 3)] * (seq_t)14;
        val += cache[(size_t)(i - 4)] * (seq_t)19;
        val += cache[(size_t)(i - 5)] * (seq_t)13;
        val -= cache[(size_t)(i - 6)] * (seq_t)22;
        val -= cache[(size_t)(i - 7)] * (seq_t)4;
        val += cache[(size_t)(i - 8)] * (seq_t)8;
        cache[(size_t)i] = val;
    }

    cache_max = n;
    return cache[(size_t)n];
}
