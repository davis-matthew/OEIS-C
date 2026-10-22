/*
 * A373228  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 8; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 8.
 * https://oeis.org/A373228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 8; a(n) = a(n-a(n-1)) + a(n-a(n-2)) +
 *            a(n-a(n-3)) for n > 8 (relative of Hofstadter Q-sequence)
 * Range:     u8 n <= 320, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;       /* array of a(i) values, indices start at 1 */
static idx_t  built   = 0;          /* highest n for which cache[n] is valid   */
static size_t cap     = 0;          /* current allocated capacity (elements)  */

/* Grow the cache to be able to hold indices up to 'n' (inclusive).  */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cap)
        return;

    size_t newcap = cap ? cap << 1 : 8;
    while ((size_t)n + 1 > newcap)
        newcap <<= 1;

#ifdef __cplusplus
    /* Allocate new array, copy old contents, deallocate old. */
    seq_t *tmp = new seq_t[newcap];
    if (!tmp && newcap > 0)
        abort();
    for (size_t i = 0; i < cap; ++i)
        tmp[i] = cache[i];
    delete[] cache;
#else
    /* In C use realloc on the raw buffer.  */
    seq_t *tmp = realloc(cache, newcap * sizeof(seq_t));
    if (!tmp && newcap > 0)
        abort();
#endif
    cache = tmp;
    cap   = newcap;
}

/* Build the sequence values up to and including index 'n'. */
static void build_up_to(idx_t n)
{
    if (n <= built)
        return;   /* already computed */

    ensure_capacity(n);

    for (idx_t k = built + 1; k <= n; ++k) {
        if (k <= 8) {                            /* initial terms */
            cache[(size_t)k] = (seq_t)k;
        } else {                                 /* recurrence */
            seq_t a1 = cache[(size_t)(k - 1)];
            seq_t a2 = cache[(size_t)(k - 2)];
            seq_t a3 = cache[(size_t)(k - 3)];

            idx_t i1 = k - (idx_t)a1;
            idx_t i2 = k - (idx_t)a2;
            idx_t i3 = k - (idx_t)a3;

            seq_t v1 = cache[(size_t)i1];
            seq_t v2 = cache[(size_t)i2];
            seq_t v3 = cache[(size_t)i3];

            cache[(size_t)k] = v1 + v2 + v3;
        }
    }
    built = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, not used */

    build_up_to(n);
    return cache[(size_t)n];
}
