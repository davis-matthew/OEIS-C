/*
 * A253880  Triangular numbers (A000217) that are also centered heptagonal
 *          numbers (A069099).
 * https://oeis.org/A253880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 254*a(n-1) - a(n-2); a(1)=1, a(2)=253
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External types defined in the build environment */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset is 1; values below are irrelevant */

    /* Static cache: cache[i] holds a(i) for i ≥ 1 */
    static seq_t *cache   = NULL;             /* pointer to cached values */
    static size_t  cap    = 0;                /* allocated capacity (index 0 unused) */
    static idx_t   computed = 0;              /* greatest index computed so far */

    /* Ensure we have enough space for indices up to max(n,2) */
    if ((size_t)n + 2 > cap) {
        size_t new_cap = (size_t)n + 2;       /* indices 0 .. n + 1 */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;             /* allocation failed – return 0 */
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialize the first two terms if not yet set */
    if (computed < 1) {
        cache[1] = (seq_t)1;
        computed = 1;
    }
    if (computed < 2) {
        cache[2] = (seq_t)253;
        computed = 2;
    }

    /* Compute required terms up to n (using the recurrence) */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)254 * cache[i-1] - cache[i-2];
    }
    computed = n;

    return cache[n];
}
