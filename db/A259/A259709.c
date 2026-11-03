/*
 * A259709  a(n) = A259708(n,n-1).
 * https://oeis.org/A259709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 6*a(n-3) + 2*a(n-4) + 4*a(n-5) +
 *            a(n-6) Initial values: a(1)=0, a(2)=-1, a(3)=0, a(4)=4, a(5)=22,
 *            a(6)=78
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 286 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* computed terms */
    static size_t computed  = 0;       /* number of terms computed */
    static size_t cache_cap = 0;       /* current capacity of cache */

    if (n == 0) return 0;             /* not part of the sequence, harmless */

    size_t idx = (size_t)n;           /* zero‑based array index is n-1 */

    /* Ensure we have storage for terms up to n */
    grow_seq(&cache, &cache_cap, idx);

    /* Compute missing terms */
    for (size_t i = computed; i < idx; ++i) {
        if (i == 0)         cache[i] = (seq_t)0;          /* a(1) */
        else if (i == 1)    cache[i] = (seq_t)-1;         /* a(2) */
        else if (i == 2)    cache[i] = (seq_t)0;          /* a(3) */
        else if (i == 3)    cache[i] = (seq_t)4;          /* a(4) */
        else if (i == 4)    cache[i] = (seq_t)22;         /* a(5) */
        else if (i == 5)    cache[i] = (seq_t)78;         /* a(6) */
        else {
            seq_t t;
            t = mul_small(cache[i-1], 4);
            t -= mul_small(cache[i-2], 2);
            t -= mul_small(cache[i-3], 6);
            t += mul_small(cache[i-4], 2);
            t += mul_small(cache[i-5], 4);
            t += cache[i-6];
            cache[i] = t;
        }
    }
    computed = idx;
    return cache[n-1];
}
