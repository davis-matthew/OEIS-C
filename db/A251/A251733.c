/*
 * A251733  a(n) = 3^n*A077985(n-1), A077985(-1) = 0. Irrational parts of the
 *          integers in Q(sqrt(2)) giving the length of a Lévy C-curve variant
 *          at iteration step n.
 * https://oeis.org/A251733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9*a(n-2) - 6*a(n-1); a(0) = 0, a(1) = 3;
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;   /* array of terms   */
    static size_t  cap   = 0;     /* allocated size   */
    static idx_t   highest = (idx_t)0;  /* highest index computed */

    /* Initial allocation for indices 0 and 1. */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        assert(cache != NULL);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        highest = 1;
    }

    /* Reallocate cache if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 2;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        assert(tmp != NULL);
        cache = tmp;
        cap = new_cap;
    }

    /* Return known values directly. */
    if (n <= highest) return cache[n];

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = highest + 1; i <= n; ++i) {
        cache[(size_t)i] = (seq_t)(9 * cache[(size_t)(i - 2)] - 6 * cache[(size_t)(i - 1)]);
    }
    highest = n;
    return cache[(size_t)n];
}
