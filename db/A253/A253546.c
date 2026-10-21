/*
 * A253546  Centered hexagonal numbers (A003215) which are also centered
 *          heptagonal numbers (A069099).
 * https://oeis.org/A253546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A253546: Centered hexagonal numbers that are also centered
   heptagonal numbers.
   Recurrence: a(1)=1, a(2)=547, a(3)=368551,
   a(n)=675*a(n-1)-675*a(n-2)+a(n-3) for n≥4. */

#include <stddef.h>
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build environment. */
extern seq_t a(idx_t n);   /* declaration for the compiler */

static seq_t *cache = NULL;     /* dynamically growing cache of terms */
static size_t cache_len = 0;    /* number of terms currently cached */

/* Main function computing a(n).  It memoises the sequence so repeated
   calls are O(1) after the first traversal up to the largest n
   requested.  The function is safe for any idx_t value that
   fits into a size_t (n ≤ 10000 in the tests). */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term.  Return 0 for n==0, which is
       outside the OEIS offset. */
    if (n == 0) return (seq_t)0;

    size_t idx = (size_t)n;  /* assume n< SIZE_MAX (true for used ranges) */

    /* Initialise first three terms as needed. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;         /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)547;
        cache[2] = (seq_t)368551;
    }

    /* Grow cache up to the requested index if necessary. */
    if (idx > cache_len) {
        size_t new_len = idx;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            seq_t v3 = cache[i - 3];
            cache[i] = (seq_t)675 * v1 - (seq_t)675 * v2 + v3;
        }
        cache_len = new_len;
    }

    /* Return the requested term (n is 1‑based, array is 0‑based). */
    return cache[idx - 1];
}
