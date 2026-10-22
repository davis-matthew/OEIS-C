/*
 * A379686  a(n) = (a(n-1)+1)*(a(n-2)+1)*(a(n-3)+1)*(a(n-4)+1)/a(n-5), with
 *          a(1)=1, a(2)=2, a(3)=6, a(4)=42, a(5)=1806.
 * https://oeis.org/A379686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A379686:  a(1)=1, a(2)=2, a(3)=6, a(4)=42, a(5)=1806
    a(n) = (a(n-1)+1)*(a(n-2)+1)*(a(n-3)+1)*(a(n-4)+1)/a(n-5)   for n ≥ 6.  */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] = a(i+1) */
static size_t cap   = 0;      /* current capacity of cache array */
static size_t sz    = 0;      /* number of terms currently stored */

static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; undefined for n < 1 */

    if (cache == NULL) {
        /* initialize with the first five terms */
        ensure_capacity(8);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)42;
        cache[4] = (seq_t)1806;
        sz = 5;
    }

    if ((size_t)n <= sz) return cache[n-1];

    ensure_capacity((size_t)n);

    for (size_t k = sz + 1; k <= (size_t)n; ++k) {
        /* k is 1‑based; use previous five terms */
        seq_t term = (cache[k-2] + (seq_t)1) *
                     (cache[k-3] + (seq_t)1) *
                     (cache[k-4] + (seq_t)1) *
                     (cache[k-5] + (seq_t)1) /
                     cache[k-6];
        cache[k-1] = term;
    }
    sz = (size_t)n;
    return cache[n-1];
}
