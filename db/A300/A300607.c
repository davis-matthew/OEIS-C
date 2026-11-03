/*
 * A300607  Number of nX3 0..1 arrays with every element equal to 1, 2, 3 or 5
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence
 *            a(n)=5*a(n-1)+5*a(n-2)-19*a(n-3)-6*a(n-4)+12*a(n-5)-2*a(n-6)+20*a(n-7)
 *            +15*a(n-8)-49*a(n-9)-35*a(n-10)+15*a(n-11)+4*a(n-12)+16*a(n-13)
 *            +36*a(n-14)+8*a(n-15)-16*a(n-16)-8*a(n-17) for n>18 with initial
 *            values a(1)=1, a(2)=10, a(3)=53, a(4)=277, a(5)=1432, a(6)=7...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 197 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static const seq_t init_values[18] = {
    (seq_t)1ULL, (seq_t)10ULL, (seq_t)53ULL, (seq_t)277ULL,
    (seq_t)1432ULL, (seq_t)7514ULL, (seq_t)39330ULL,
    (seq_t)205958ULL, (seq_t)1078479ULL, (seq_t)5647607ULL,
    (seq_t)29574290ULL, (seq_t)154869427ULL,
    (seq_t)810992627ULL, (seq_t)4246863308ULL,
    (seq_t)22239224467ULL, (seq_t)116458451229ULL,
    (seq_t)609849090084ULL, (seq_t)3193550233566ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;   /* largest n computed and stored (1‑based) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe fallback – OEIS offset is 1 */

    if (n <= 18) return init_values[n - 1];

    /* initialise cache if not already done */
    if (cache_len == 0) {
        /* we need space for indices up to 18 (1‑based), plus a dummy 0 */
        ensure_cache((idx_t)(18 + 1), &cache, &cache_cap);
        for (idx_t i = 1; i <= 18; ++i) cache[i] = init_values[i - 1];
        cache_len = 18;
    }

    if ((size_t)n > cache_cap) {
        /* grow cache to hold indices up to n (plus a dummy 0) */
        ensure_cache((idx_t)(n + 1), &cache, &cache_cap);
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* use separated positive and negative sums to avoid unsigned wrap */
        seq_t pos = 0, neg = 0;
        /* coefficient list for a(i) in terms of a(i-1) .. a(i-17) */
        const int coeffs[17] = {
            5, 5, -19, -6, 12, -2, 20, 15,
            -49, -35, 15, 4, 16, 36, 8, -16, -8
        };
        for (int k = 0; k < 17; ++k) {
            seq_t term = cache[i - 1 - k];
            int c = coeffs[k];
            if (c > 0)
                pos += term * ((seq_t)c);
            else
                neg += term * ((seq_t)(-c));
        }
        cache[i] = pos - neg;   /* pos >= neg for all valid n */
    }
    cache_len = n;
    return cache[n];
}
