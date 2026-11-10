/*
 * A351809  a(0) = 32; then, for n >= 1, a(n) is the smallest positive integer
 *          k such that pod(k^2)/pod(k) = A002473(n) where pod = product of
 *          digits = A007954.
 * https://oeis.org/A351809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k with pod(k)>0 and pod(k^2)/pod(k) = A002473(n)
 *            (a(0): ratio 0, giving 32); by incremental search with memoized
 *            first hits per ratio
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A351809_ratio = NULL;
static uint64_t *A351809_kval = NULL;
static size_t A351809_cnt = 0;
static size_t A351809_cap = 0;
static uint64_t A351809_next = 1;

static uint64_t A351809_pod(uint64_t x) {
    if (x == 0) return 0;
    uint64_t r = 1;
    while (x > 0) {
        uint64_t d = x % 10;
        if (d == 0) return 0;
        r *= d;
        x /= 10;
    }
    return r;
}

static uint64_t A351809_smooth(uint64_t n) {
    uint64_t *s = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    uint64_t i2 = 0, i3 = 0, i5 = 0, i7 = 0, len = 1, res;
    s[0] = 1;
    while (len < n) {
        uint64_t a2 = s[i2] * 2, a3 = s[i3] * 3, a5 = s[i5] * 5, a7 = s[i7] * 7;
        uint64_t m = a2;
        if (a3 < m) m = a3;
        if (a5 < m) m = a5;
        if (a7 < m) m = a7;
        s[len++] = m;
        if (m == a2) i2++;
        if (m == a3) i3++;
        if (m == a5) i5++;
        if (m == a7) i7++;
    }
    res = s[n - 1];
    free(s);
    return res;
}

static int A351809_find(uint64_t t, uint64_t *out) {
    size_t i;
    for (i = 0; i < A351809_cnt; i++) {
        if (A351809_ratio[i] == t) { *out = A351809_kval[i]; return 1; }
    }
    return 0;
}

seq_t a(idx_t n) {
    uint64_t t = 0, found;
    if (n >= 1) t = A351809_smooth((uint64_t)n);
    for (;;) {
        if (A351809_find(t, &found)) return (seq_t)found;
        {
            uint64_t k = A351809_next++;
            uint64_t pk = A351809_pod(k);
            if (pk != 0) {
                uint64_t ps = A351809_pod(k * k);
                if (ps % pk == 0) {
                    uint64_t r = ps / pk, dummy;
                    if (!A351809_find(r, &dummy)) {
                        if (A351809_cnt == A351809_cap) {
                            size_t nc = A351809_cap ? A351809_cap * 2 : 64;
                            A351809_ratio = (uint64_t *)realloc(A351809_ratio, nc * sizeof(uint64_t));
                            A351809_kval = (uint64_t *)realloc(A351809_kval, nc * sizeof(uint64_t));
                            A351809_cap = nc;
                        }
                        A351809_ratio[A351809_cnt] = r;
                        A351809_kval[A351809_cnt] = k;
                        A351809_cnt++;
                    }
                }
            }
        }
    }
}
