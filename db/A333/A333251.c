/*
 * A333251  Tropical version of Somos-5 sequence A006721.
 * https://oeis.org/A333251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = max(a(n-1)+a(n-4), a(n-2)+a(n-3)) - a(n-5) a(0) = -1,
 *            a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0
 * Range:     i8 n <= 59, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* array storing computed terms      */
static size_t sz = 0;          /* number of terms currently stored   */
static size_t cap = 0;         /* allocated capacity (in terms)       */

static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    size_t new_cap = (cap == 0) ? 16 : cap << 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* In the unlikely event of memory exhaustion, abort. */
        abort();
    }
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise first five terms lazily */
    if (sz == 0) {
        ensure_capacity(5);
        cache[0] = (seq_t)-1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        sz = 5;
    }

    if ((idx_t)sz > n)
        return cache[n];

    ensure_capacity((size_t)(n + 1));

    for (idx_t i = sz; i <= n; ++i) {
        seq_t t1 = cache[i - 1] + cache[i - 4];
        seq_t t2 = cache[i - 2] + cache[i - 3];
        seq_t m  = (t1 > t2) ? t1 : t2;
        cache[i] = m - cache[i - 5];
    }
    sz = n + 1;
    return cache[n];
}
