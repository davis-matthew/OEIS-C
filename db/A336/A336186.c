/*
 * A336186  Side length of a square block of integers, with 1 at the top-left
 *          corner, on a diagonally numbered 2D board such that the sum of the
 *          integers in the square is a perfect square.
 * https://oeis.org/A336186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 110*a(n-2) - a(n-4) for n>4, a(1)=1, a(2)=17, a(3)=127,
 *            a(4)=1871
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

/* Compute a(n) for the OEIS sequence A336186. */
seq_t a(idx_t n)
{
    /* Static cache to memoise computed values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* current allocated capacity (>=1) */
    static size_t computed = 0;  /* largest n for which cache[n] is valid */

    if (n == 0) return (seq_t)0; /* undefined, but safe */

    /* Ensure the cache can hold up to n (1‑based). */
    size_t need = (size_t)n + 1; /* include index 0 unused */
    if (need > cap)
        grow_seq(&cache, &cap, need);

    /* Initialise the first four terms if they are not yet set. */
    if (computed < 4) {
        if (cache[1] == 0) cache[1] = (seq_t)1;
        if (computed < 2 && cache[2] == 0) cache[2] = (seq_t)17;
        if (computed < 3 && cache[3] == 0) cache[3] = (seq_t)127;
        if (computed < 4 && cache[4] == 0) cache[4] = (seq_t)1871;
        computed = 4;
    }

    /* Compute needed terms using the recurrence. */
    for (size_t i = (computed >= 4 ? computed + 1 : 5); i <= (size_t)n; ++i) {
        cache[i] = cache[i - 2] * (seq_t)110 - cache[i - 4];
    }
    computed = (size_t)n;

    return cache[(size_t)n];
}
