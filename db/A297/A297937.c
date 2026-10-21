/*
 * A297937  Number of nX2 0..1 arrays with every element equal to 0, 2, 3 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 4*a(n-4) (n > 5) Initial values:
 *            a(1)=1, a(2)=5, a(3)=7, a(4)=18, a(5)=31 The function memoises
 *            results in a static cache so that any order of calls works
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] unused; cache[i] = a(i) for i>=1 */
static size_t cap   = 0;      /* allocated size (indices 0 .. cap-1) */
static size_t computed = 0;   /* largest n that has been computed */

/* Ensure that cache can hold up to n (n >= 1) and that a(1)..a(n) are computed */
static void ensure_a(idx_t n)
{
    if (n < 1) return;                      /* nothing to do for invalid n */

    /* At least space for indices 0..5 (to store the 5 known initial terms) */
    size_t required = (size_t)(n + 1);
    if (required < 6) required = 6;

    /* Grow the allocation if necessary */
    if (required > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failure */
        cache = tmp;
        cap   = required;
    }

    /* Initialise base values the first time */
    if (computed == 0) {
        const seq_t base[6] = {0, 1, 5, 7, 18, 31}; /* base[0] unused */
        for (size_t i = 1; i <= 5; ++i)
            cache[i] = base[i];
        computed = 5;
    }

    if ((size_t)n <= computed) return;      /* already computed up to n */

    /* Extend computation up to n */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        /* For i <= 5 the values are already set; recurrence starts at i = 6 */
        if (i <= 5) continue;
        seq_t term1 = cache[i - 1];
        seq_t term2 = 3 * cache[i - 2];
        seq_t term3 = 4 * cache[i - 4];
        cache[i] = term1 + term2 - term3;
    }
    computed = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n < 1, safe default */
    ensure_a(n);
    return cache[(size_t)n];
}
