/*
 * A343273  a(n) is the number of geometrically distinct edge-unfoldings of
 *          the regular n-gonal cupola.
 * https://oeis.org/A343273
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 a(n-1) - a(n-2) -109 a(n-3) +109 a(n-4) + a(n-5) -11
 *            a(n-6) + a(n-7) for n >= 10, with initial values a(3) = 308,
 *            a(4) = 3030, a(5) = 29757, a(6) = 294327, a(7) = 2911142, a(8) =
 *            28814940, a(9) = 285214743. For n < 3, a(n) = 0
 * Range:     u8 none, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* local cache for computed a(n) values */
static seq_t *a_cache = NULL;
static idx_t a_current = 0;      /* highest n for which a_cache[n] is valid */
static size_t a_capacity = 0;

static void ensure_a_cache(idx_t n)
{
    if (n <= a_current)
        return;

    /* grow cache if necessary */
    if (a_cache == NULL || a_capacity <= (size_t)n) {
        size_t new_cap = a_capacity ? a_capacity * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            abort();
        a_cache = tmp;
        a_capacity = new_cap;
    }

    /* initialise first few values if not yet done */
    if (a_current < 3) {
        a_cache[0] = 0;
        a_cache[1] = 0;
        a_cache[2] = 0;
        a_cache[3] = (seq_t)308;
        a_current = 3;
    }
    if (a_current < 4 && n >= 4)
        a_cache[4] = (seq_t)3030, a_current = 4;
    if (a_current < 5 && n >= 5)
        a_cache[5] = (seq_t)29757, a_current = 5;
    if (a_current < 6 && n >= 6)
        a_cache[6] = (seq_t)294327, a_current = 6;
    if (a_current < 7 && n >= 7)
        a_cache[7] = (seq_t)2911142, a_current = 7;
    if (a_current < 8 && n >= 8)
        a_cache[8] = (seq_t)28814940, a_current = 8;
    if (a_current < 9 && n >= 9)
        a_cache[9] = (seq_t)285214743, a_current = 9;

    /* compute new terms using the recurrence */
    for (idx_t k = a_current + 1; k <= n; ++k) {
        if (k <= 9) {
            /* already initialized */
        } else {
            seq_t val =
                (seq_t)11 * a_cache[k - 1]
                - a_cache[k - 2]
                - (seq_t)109 * a_cache[k - 3]
                + (seq_t)109 * a_cache[k - 4]
                + a_cache[k - 5]
                - (seq_t)11 * a_cache[k - 6]
                + a_cache[k - 7];
            a_cache[k] = val;
        }
    }
    a_current = n;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    ensure_a_cache(n);
    return a_cache[n];
}
