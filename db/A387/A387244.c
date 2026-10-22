/*
 * A387244  Expansion of e.g.f. exp(x^2/(1-x)^4).
 * https://oeis.org/A387244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} binomial(n + 2*k - 1, 4*k - 1)/k!
 *            (equivalently the 5th order recurrence) a(0)=1, a(1)=0, a(2)=2,
 *            a(3)=24, a(4)=252
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* dynamically filled cache */
static idx_t  max_cached = 0;     /* largest index currently cached */
static size_t cache_capacity = 0; /* current allocated capacity */

static void ensure_capacity(idx_t n)
{
    if (cache_capacity <= (size_t)n) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        cache_capacity = 5;
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)0;          /* a(1) */
        cache[2] = (seq_t)2;          /* a(2) */
        cache[3] = (seq_t)24;         /* a(3) */
        cache[4] = (seq_t)252;        /* a(4) */
        max_cached = 4;
    }

    if (n <= max_cached) {
        return cache[n];
    }

    ensure_capacity(n);

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* recurrence only applicable for i >= 5 */
        seq_t term;
        /* Use explicit casts to seq_t for each operand */
        seq_t i1 = (seq_t)(i - 1);
        seq_t i2 = (seq_t)(i - 2);
        seq_t i3 = (seq_t)(i - 3);
        seq_t i4 = (seq_t)(i - 4);
        seq_t i5 = (seq_t)(i - 5);

        seq_t t1 = (seq_t)5 * i1 * cache[i - 1];
        seq_t t2 = (seq_t)2 * i1 * ((seq_t)5 * i - (seq_t)11) * cache[i - 2];
        seq_t t3 = (seq_t)2 * i2 * i1 * ((seq_t)5 * i - (seq_t)14) * cache[i - 3];
        seq_t t4 = (seq_t)5 * i4 * i3 * i2 * i1 * cache[i - 4];
        seq_t t5 = i5 * i4 * i3 * i2 * i1 * cache[i - 5];

        term = t1 - t2 + t3 - t4 + t5;
        cache[i] = term;
    }

    max_cached = n;
    return cache[n];
}
