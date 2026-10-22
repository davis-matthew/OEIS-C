/*
 * A301781  Number of nX5 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally or vertically adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            24a(n-1)+92a(n-2)-156a(n-3)-1088a(n-4)-480a(n-5)+3200a(n-6)+3200a(n-7)-4608a(n-8)
 *            (a(1)=16, a(2)=512, a(3)=13616, a(4)=369936, a(5)=10033408,
 *            a(6)=272151040, a(7)=7381982784, a(8)=200232929792)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[8] = {
    (seq_t)16ULL,
    (seq_t)512ULL,
    (seq_t)13616ULL,
    (seq_t)369936ULL,
    (seq_t)10033408ULL,
    (seq_t)272151040ULL,
    (seq_t)7381982784ULL,
    (seq_t)200232929792ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* The sequence starts at index 1. Return 0 for n==0 to avoid UB. */
    if (n == 0) return (seq_t)0;

    /* Determine the number of terms that must exist in the cache. */
    const size_t target = (size_t)n;

    if (cache_cap < target) {
        /* Expand the cache. */
        seq_t *new_cache = (seq_t *)realloc(cache, target * sizeof(seq_t));
        if (!new_cache) abort(); /* Out of memory. */
        cache = new_cache;

        for (size_t i = cache_cap; i < target; ++i) {
            if (i < 8) {
                /* Initial seed values. */
                cache[i] = init_vals[i];
                continue;
            }
            /* Indices of the 8 previous terms. */
            size_t a1 = i - 1;
            size_t a2 = i - 2;
            size_t a3 = i - 3;
            size_t a4 = i - 4;
            size_t a5 = i - 5;
            size_t a6 = i - 6;
            size_t a7 = i - 7;
            size_t a8 = i - 8;

            /* Compute the recurrence. */
            seq_t pos = (seq_t)0;
            pos += cache[a1] * (seq_t)24;
            pos += cache[a2] * (seq_t)92;
            pos += cache[a6] * (seq_t)3200;
            pos += cache[a7] * (seq_t)3200;

            seq_t neg = (seq_t)0;
            neg += cache[a3] * (seq_t)156;
            neg += cache[a4] * (seq_t)1088;
            neg += cache[a5] * (seq_t)480;
            neg += cache[a8] * (seq_t)4608;

            /* The recurrence always yields a non‑negative result. */
            cache[i] = pos - neg;
        }

        cache_cap = target;
    }

    return cache[n - 1];
}
