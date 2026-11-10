/*
 * A361750  Terms of A329150 that have several preimages.
 * https://oeis.org/A361750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number v>0 whose decimal string can be split into
 *            tokens {0,2,3,5,7,11,13,17,19,23} (images of digits under
 *            A329147) in at least two ways; computed by a linear search with
 *            a parse-counting DP
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A361750_multi(seq_t v)
{
    int d[48];
    int len = 0;
    seq_t x = v;
    while (x > 0) {
        d[len++] = (int)(x % 10);
        x /= 10;
    }
    /* reverse to most-significant first */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        int t = d[i]; d[i] = d[j]; d[j] = t;
    }
    int f[49];
    f[0] = 1;
    for (int i = 1; i <= len; ++i) {
        int c = 0;
        int g = d[i - 1];
        if (g == 0 || g == 2 || g == 3 || g == 5 || g == 7) {
            if (!(g == 0 && i == 1)) c += f[i - 1];
        }
        if (i >= 2) {
            int p = d[i - 2] * 10 + g;
            if (p == 11 || p == 13 || p == 17 || p == 19 || p == 23) c += f[i - 2];
        }
        if (c > 2) c = 2;
        f[i] = c;
    }
    return f[len] >= 2;
}

seq_t a(idx_t n)
{
    static idx_t cache_n = 0;
    static seq_t cache_v = 0;
    idx_t k;
    seq_t v;
    if (cache_n != 0 && n >= cache_n) {
        k = cache_n;
        v = cache_v;
    } else {
        k = 0;
        v = 0;
    }
    while (k < n) {
        ++v;
        if (A361750_multi(v)) ++k;
    }
    cache_n = k;
    cache_v = v;
    return v;
}
