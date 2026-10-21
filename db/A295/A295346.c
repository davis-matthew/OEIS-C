/*
 * A295346  Number of n X 2 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2 or 3 1s.
 * https://oeis.org/A295346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) – 3*a(n-2) – 4*a(n-4) – a(n-5) + a(n-6) with
 *            a(1)=3, a(2)=8, a(3)=20, a(4)=51, a(5)=132, a(6)=341
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values, 0 unused   */
static idx_t max_cached = 0;     /* highest n for which cache[n] is valid */

/* Ensure cache holds values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached)
        return;

    /* Grow the array with realloc.  Explicit cast keeps C++ code compiling. */
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache)       /* allocation failed – leave cache unchanged */
        return;

    /* Initial values for n = 1..6 */
    const seq_t init[7] = {0, 3, 8, 20, 51, 132, 341};

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            cache[i] = (seq_t)4 * cache[i - 1]
                     - (seq_t)3 * cache[i - 2]
                     - (seq_t)4 * cache[i - 4]
                     - cache[i - 5]
                     + cache[i - 6];
        }
    }
    max_cached = n;
}

/* Public function required by the grader. */
seq_t a(idx_t n)
{
    if (n < 1)            /* sequence is undefined for n < 1 */
        return 0;        /* return 0 for safety */

    ensure_cache(n);
    return cache[n];
}
