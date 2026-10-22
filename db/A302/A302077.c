/*
 * A302077  Number of nX4 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A302077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A302077
    Number of n×4 0..1 arrays with every element equal to 0, 1, 3 or 4
    horizontally or antidiagonally adjacent elements, with upper left
    element zero.
    Empirical recurrence (order 8):
        a(n) = a(n-1) + 14*a(n-2) - a(n-3) - 42*a(n-4)
               - a(n-5) + 14*a(n-6) + a(n-7) - a(n-8)   (n ≥ 9)
    Initial values:
        a(1)=5,  a(2)=18,  a(3)=70,  a(4)=265,  a(5)=1018,
        a(6)=3914, a(7)=15017, a(8)=57850
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;           /* 1‑based array of computed values   */
static size_t cache_cap = 0;          /* number of allocated slots (≥ n+1) */
static size_t cache_len = 0;          /* highest index that has been computed */

/* Ensure that cache has space for at least n+1 elements (1‑indexed) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1); /* include unused index 0 */
    if (cache_cap >= needed) return;

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) abort();          /* memory failure */
    cache = new_cache;
    cache_cap = needed;
}

/* Return a(n) for n ≥ 1, any calling order (memoised). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but safe */

    /* Initialise cache and first eight terms on first call */
    if (cache == NULL) {
        ensure_cache(8);
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)70;
        cache[4] = (seq_t)265;
        cache[5] = (seq_t)1018;
        cache[6] = (seq_t)3914;
        cache[7] = (seq_t)15017;
        cache[8] = (seq_t)57850;
        cache_len = 8;
    }

    if (n <= cache_len) return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t term =
            cache[i-1]                               /* +a(n-1) */
          + (seq_t)14 * cache[i-2]                   /* +14*a(n-2) */
          - cache[i-3]                               /* -a(n-3) */
          - (seq_t)42 * cache[i-4]                   /* -42*a(n-4) */
          - cache[i-5]                               /* -a(n-5) */
          + (seq_t)14 * cache[i-6]                   /* +14*a(n-6) */
          + cache[i-7]                               /* +a(n-7) */
          - cache[i-8];                              /* -a(n-8) */
        cache[i] = term;
    }

    cache_len = n;
    return cache[n];
}
