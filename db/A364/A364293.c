/*
 * A364293  Odd numbers k for which A163511(k) <= k.
 * https://oeis.org/A364293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k such that A163511(k) <= k, where A163511(k) =
 *            p(r) * Prod_{i=1..r} p(i)^(c_i - c_{i-1} - 1), r = A000120(k),
 *            c_i = position of i-th set bit of k (1-based from LSB), c_0 = 0.
 *            Computed by scanning odd k in increasing order (cached)
 * Range:     u8 n <= 21, u16 n <= 756, u32 n <= 15157, u64 n <= 15157,
 *            u128 n <= 15157, big any n
 * Validated: standard, pass, 77 terms through n = 9468 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A163511_le(uint64_t k)
{
    static uint32_t pr[64];
    static int have = 0;
    if (!have) {
        int c = 0;
        uint32_t x = 2;
        while (c < 64) {
            int ok = 1;
            for (int i = 0; i < c; i++) {
                if (pr[i] * pr[i] > x) break;
                if (x % pr[i] == 0) { ok = 0; break; }
            }
            if (ok) pr[c++] = x;
            x++;
        }
        have = 1;
    }
    int r = 0;
    uint64_t t = k;
    while (t) { r += (int)(t & 1); t >>= 1; }
    uint64_t res = pr[r - 1];
    if (res > k) return 0;
    int prev = 0, bit = 0, i = 0;
    t = k;
    while (t) {
        bit++;
        if (t & 1) {
            int e = bit - prev - 1;
            for (int j = 0; j < e; j++) {
                if (res > k / pr[i]) return 0;
                res *= pr[i];
            }
            prev = bit;
            i++;
        }
        t >>= 1;
    }
    return res <= k;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t nextk = 1;
    while (cnt < (size_t)n) {
        if (A163511_le(nextk)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                cache = (uint64_t *)realloc(cache, cap * sizeof(uint64_t));
            }
            cache[cnt++] = nextk;
        }
        nextk += 2;
    }
    return (seq_t)cache[n - 1];
}
