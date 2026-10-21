/*
 * A295684  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 0, a(2) =
 *          1, a(3) = 1.
 * https://oeis.org/A295684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-3) + a(n-4)
 * Initial values: a(0) = 2, a(1) = 0, a(2) = 1, a(3) = 1
 *
 * Computes the OEIS sequence A295684 for any n ≥ 0, memoizing results so
 * that each term is evaluated only once.  The function works with any
 * integer type `seq_t` provided by the build, including arbitrary‐precision
 * types, and is safe for repeated, out‑of‑order calls.
 */
#include <stddef.h>    /* for size_t */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Persistent cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity (elements)   */
    static size_t used  = 0;   /* number of terms actually computed */

    /* Helper to grow the cache to at least `min_cap` entries */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (cap == 0) ? 4 : cap;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;          /* double until sufficient */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable memory error */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base cases if no terms known yet or cache was too small */
    if (used < 4) {
        if (cap > 0)      cache[0] = (seq_t)2;
        if (cap > 1)      cache[1] = (seq_t)0;
        if (cap > 2)      cache[2] = (seq_t)1;
        if (cap > 3)      cache[3] = (seq_t)1;
        used = (used < 4) ? 4 : used;
    }

    /* Compute missing terms up to index n (inclusive) */
    if ((size_t)n >= used) {
        for (size_t i = used; i <= (size_t)n; ++i) {
            /* a(i) = a(i-1) + a(i-3) + a(i-4) */
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
        used = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
