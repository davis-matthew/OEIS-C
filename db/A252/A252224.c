/*
 * A252224  Number of (n+2) X (4+2) 0..2 arrays with every 3 X 3 subblock row
 *          and column sum 2 3 or 4 and every diagonal and antidiagonal sum
 *          not 2 3 or 4.
 * https://oeis.org/A252224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 14, u64 n <= 33, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A252224
    Sequence: Number of (n+2)×(6) arrays with entries 0..2 such that every
    3×3 subblock has its row– and column–sums in {2,3,4}, while all
    main–diagonals and antidiagonals avoid these sums.

    Empirical recurrence (valid for n>8):
        a(n) = 3*a(n-1) - a(n-2) + 3*a(n-3) + 3*a(n-4)
               - a(n-5) - a(n-6)

    Initial values (n = 1 .. 8):
        1442, 7724, 24932, 58334, 160688, 512822, 1619714, 4972874
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* Static cache for already computed terms, 1‑based indexing. */
    static seq_t *cache = NULL;
    static size_t cap  = 0;                  /* current capacity of cache */
    static bool init_done = false;           /* have we initialised the head? */

    /* Ensure the cache is large enough for index n (1‑based). */
    if (cache == NULL || cap <= (size_t)n) {
        size_t new_cap = (cache == NULL) ? 16 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* grow by power‑of‑two */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cap = new_cap;
    }

    /* Store the first 8 terms (for all builds) the first time we run. */
    if (!init_done) {
        static const seq_t init_vals[9] = {
            0,          /* unused index 0 */
            1442, 7724, 24932, 58334, 160688,
            512822, 1619714, 4972874
        };
        for (size_t i = 1; i <= 8; ++i) {
            cache[i] = init_vals[i];
        }
        init_done = true;
    }

    /* If the requested n is within the pre‑filled range, just return. */
    if (n <= 8) return cache[(size_t)n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = 9; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t val = 3 * cache[idx-1];
        val -= cache[idx-2];
        val += 3 * cache[idx-3];
        val += 3 * cache[idx-4];
        val -= cache[idx-5];
        val -= cache[idx-6];
        cache[idx] = val;
    }

    return cache[(size_t)n];
}
