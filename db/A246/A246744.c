/*
 * A246744  Number of length 7+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a linear recurrence of order 21: a(n) = 3*a(n-1)
 *            +6*a(n-2) -26*a(n-3) -9*a(n-4) +99*a(n-5) -24*a(n-6) -216*a(n-7)
 *            +126*a(n-8) +294*a(n-9) -252*a(n-10) -252*a(n-11) +294*a(n-12)
 *            +126*a(n-13) -216*a(n-14) -24*a(n-15) +99*a(n-16) -9*a(n-17)
 *            -26*a(n-18) +6*a(n-19) +3*a(n-20) -a(n-21) w...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 55 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Determine a(n) for 1 <= n <= 21 by counting arrays of length 11
   over alphabet {0,..,n} with no pair in any consecutive five terms
   summing to n. */
static seq_t count_initial(idx_t n)
{
    const unsigned int N = (unsigned int)n;
    const size_t base = (size_t)(N + 1);
    const size_t sz = base * base * base * base;

    typedef unsigned __int128 u128;
    u128 *cur = (u128 *)calloc(sz, sizeof(u128));
    u128 *nxt = (u128 *)calloc(sz, sizeof(u128));
    if (!cur || !nxt) {
        if (cur) free(cur);
        if (nxt) free(nxt);
        return (seq_t)0;
    }

    /* All 4‑tuples are initially possible */
    for (size_t i = 0; i < sz; ++i) cur[i] = 1;

    /* Append positions 5 through 11 */
    for (size_t pos = 5; pos <= 11; ++pos) {
        /* zero next array */
        for (size_t i = 0; i < sz; ++i) nxt[i] = 0;

        for (size_t idx = 0; idx < sz; ++idx) {
            u128 cnt = cur[idx];
            if (!cnt) continue;

            /* decode the last four values a,b,c,d */
            size_t tmp = idx;
            unsigned int d = (unsigned int)(tmp % base); tmp /= base;
            unsigned int c = (unsigned int)(tmp % base); tmp /= base;
            unsigned int b = (unsigned int)(tmp % base); tmp /= base;
            unsigned int a = (unsigned int)tmp;

            for (unsigned int v = 0; v <= N; ++v) {
                /* check that no pair among (a,b,c,d,v) sums to N */
                if (a + v == N) continue;
                if (b + v == N) continue;
                if (c + v == N) continue;
                if (d + v == N) continue;
                if (a + b == N) continue;
                if (a + c == N) continue;
                if (a + d == N) continue;
                if (b + c == N) continue;
                if (b + d == N) continue;
                if (c + d == N) continue;

                size_t nidx = ((((size_t)b * base + c) * base + d) * base + v);
                nxt[nidx] += cnt;
            }
        }
        /* swap */
        u128 *tmp_ptr = cur;
        cur = nxt;
        nxt = tmp_ptr;
    }

    /* sum all remaining states */
    u128 total = 0;
    for (size_t i = 0; i < sz; ++i) total += cur[i];

    free(cur);
    free(nxt);
    return (seq_t)total;
}

static const int32_t coeff[21] = {
    3, 6, -26, -9, 99, -24, -216, 126, 294, -252, -252,
    294, 126, -216, -24, 99, -9, -26, 6, 3, -1
};

/* Memoised sequence a(n) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* indices 1.. */
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            if (cache) mem_free(cache);
            cache = NULL;
            cache_cap = 0;
            return (seq_t)0;
        }
        if (tmp != cache) cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* compute sequentially up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 21) {
            cache[i] = count_initial(i);
        } else {
            __int128 sum = 0;
            for (int k = 1; k <= 21; ++k) {
                sum += coeff[k - 1] * ( (__int128)cache[i - k] );
            }
            cache[i] = (seq_t)sum;
        }
        max_computed = i;
    }

    return cache[n];
}
