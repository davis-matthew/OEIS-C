/*
 * A257956  Row sums of A232642, when seen as a triangle read by rows.
 * https://oeis.org/A257956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+a(n-2)-4*a(n-3)-2*a(n-4) for n > 4; a(1)=1,
 *            a(2)=6, a(3)=24, a(4)=74
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] corresponds to a(1) */
static idx_t cache_count = 0;    /* number of terms stored in cache (1‑based) */
static size_t cache_cap   = 0;    /* current allocation size */

/* Ensure the cache array can hold at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if ((size_t)needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    void *tmp = realloc(cache, new_cap * sizeof(seq_t));
    cache = (seq_t*)tmp;
    cache_cap = new_cap;
}

/* Store the first four terms if not already stored. */
static void init_base_terms(void)
{
    if (cache_count >= 4)
        return;
    ensure_capacity(4);
    if (cache_count == 0) {
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)6;  /* a(2) */
        cache[2] = (seq_t)24; /* a(3) */
        cache[3] = (seq_t)74; /* a(4) */
        cache_count = 4;
    } else {
        /* If we had fewer than 4 stored, fill the rest up to 4 */
        while (cache_count < 4) {
            seq_t val;
            switch (cache_count) {
                case 0: val = (seq_t)1;  break;
                case 1: val = (seq_t)6;  break;
                case 2: val = (seq_t)24; break;
                case 3: val = (seq_t)74; break;
                default: val = (seq_t)0; break;
            }
            cache[cache_count] = val;
            cache_count++;
        }
    }
}

/* Compute terms up to index 'n' (1‑based). */
static void compute_up_to(idx_t n)
{
    init_base_terms();           /* make sure 1..4 are stored */
    if (n <= cache_count)
        return;                  /* already computed */

    ensure_capacity(n);          /* enlarge cache if necessary */

    for (idx_t i = cache_count + 1; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1)+a(i-2)-4*a(i-3)-2*a(i-4) */
        seq_t term = (seq_t)3 * cache[i - 2] + cache[i - 3];
        term -= (seq_t)4 * cache[i - 4];
        term -= (seq_t)2 * cache[i - 5];
        cache[i - 1] = term;
    }
    cache_count = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, return 0 as sentinel */
    compute_up_to(n);
    return cache[n - 1];
}
