/*
 * A285582  a(1) = 1, a(2) = a(3) = a(4) = 2; a(n) = a(a(n-3)-1) +
 *          a(n-a(n-4)-1) for n > 4.
 * https://oeis.org/A285582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 652, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=a(3)=a(4)=2; a(n)=a(a(n-3)-1)+a(n-a(n-4)-1) for n>4 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based array of computed values */
static idx_t  computed = 0;   /* largest n that has been computed */
static size_t cap = 0;        /* current capacity of cache (indexable 0..cap-1) */

seq_t a(idx_t n)
{
    /* ignore indices < 1 */
    if (n < 1) return 0;

    /* allocate enough space for indices 0 .. n (index 0 unused) */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof *cache);
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = needed;
    }

    /* initialise the fixed first terms (safe to write each time) */
    if (cap > 1) cache[1] = 1;                 /* a(1) */
    if (cap > 2) cache[2] = 2;                 /* a(2) */
    if (cap > 3) cache[3] = 2;                 /* a(3) */
    if (cap > 4) cache[4] = 2;                 /* a(4) */

    /* compute all missing terms up to n */
    idx_t start = computed + 1;
    if (start < 5) start = 5;                 /* base terms already set */
    for (idx_t i = start; i <= n; ++i) {
        idx_t j1 = (idx_t)cache[i - 3] - 1;    /* a(i-3) - 1 */
        idx_t j2 = i - (idx_t)cache[i - 4] - 1;/* i - a(i-4) - 1 */
        cache[i] = cache[j1] + cache[j2];
    }

    if (n > computed) computed = n;
    return cache[n];
}
