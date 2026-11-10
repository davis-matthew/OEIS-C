/*
 * A333265  Least number k that reaches 1 after n iterations of A071324.
 * https://oeis.org/A333265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k : A333262(k) = n}; computed by a segmented sieve of
 *            A071324(k) = sum_{d|k, d^2<k} (-1)^(i-1) (k/d_i - d_i) (squares
 *            handled directly), with steps(k) = steps(A071324(k)) + 1,
 *            steps(1) = 0, scanning k upward and recording the first k
 *            reaching each step count. Since A071324(k) >= k/2, o...
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 56 terms through n = 55 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define S265_B (1 << 17)

static uint8_t *s265_win = NULL;   /* steps for k in [s265_wbase, s265_wbase + s265_wcap) */
static uint64_t s265_wbase = 0;
static uint64_t s265_wcap = 0;
static uint64_t s265_done = 2;     /* all k < s265_done processed */
static uint64_t s265_first[256];
static bool s265_have[256];
static bool s265_init = false;

static int32_t s265_acc[S265_B];
static int8_t s265_sg[S265_B];

static void s265_setup(void)
{
    memset(s265_have, 0, sizeof(s265_have));
    s265_wcap = (uint64_t)1 << 20;
    s265_wbase = 0;
    s265_win = (uint8_t *)calloc((size_t)s265_wcap, 1);
    s265_win[1] = 0;
    s265_first[0] = 1;
    s265_have[0] = true;
    s265_done = 2;
    s265_init = true;
}

static int s265_cmp_u64(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

/* A071324(r^2) directly from the divisors of r^2. */
static int64_t s265_square_f(uint64_t r)
{
    uint64_t pr[16];
    int ex[16];
    int c = 0;
    uint64_t m = r;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            pr[c] = p; ex[c] = 2 * e; c++;
        }
    }
    if (m > 1) { pr[c] = m; ex[c] = 2; c++; }

    size_t cnt = 1;
    for (int i = 0; i < c; i++) cnt *= (size_t)(ex[i] + 1);
    uint64_t *dv = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t nd = 1;
    dv[0] = 1;
    for (int i = 0; i < c; i++) {
        size_t cur = nd;
        uint64_t pp = 1;
        for (int e = 1; e <= ex[i]; e++) {
            pp *= pr[i];
            for (size_t j = 0; j < cur; j++) dv[nd++] = dv[j] * pp;
        }
    }
    qsort(dv, nd, sizeof(uint64_t), s265_cmp_u64);
    int64_t s = 0;
    int sign = 1;
    for (size_t j = nd; j-- > 0;) {
        s += sign * (int64_t)dv[j];
        sign = -sign;
    }
    free(dv);
    return s < 0 ? -s : s;
}

static void s265_block(void)
{
    uint64_t lo = s265_done, hi = lo + S265_B;

    /* Make room: keep only [lo/2, ...) since A071324(k) >= k/2. */
    if (hi > s265_wbase + s265_wcap) {
        uint64_t nb = lo / 2;
        if (nb > s265_wbase) {
            memmove(s265_win, s265_win + (size_t)(nb - s265_wbase), (size_t)(lo - nb));
            s265_wbase = nb;
        }
        if (hi - s265_wbase > s265_wcap) {
            uint64_t need = hi - s265_wbase;
            uint64_t nc = need + need / 2;
            s265_win = (uint8_t *)realloc(s265_win, (size_t)nc);
            s265_wcap = nc;
        }
    }

    for (size_t i = 0; i < S265_B; i++) { s265_acc[i] = 0; s265_sg[i] = 1; }

    for (uint64_t d = 1; d * d < hi; d++) {
        uint64_t s = d * d;
        if (s < lo) s = lo;
        uint64_t q = (s + d - 1) / d;
        uint64_t k = q * d;
        if (k >= hi) continue;
        int32_t t = (int32_t)(q - d);
        size_t step = (size_t)d;
        for (size_t i = (size_t)(k - lo); i < S265_B; i += step, t++) {
            s265_acc[i] += (int32_t)s265_sg[i] * t;
            s265_sg[i] = (int8_t)(-s265_sg[i]);
        }
    }

    /* Squares: override with direct computation. */
    {
        uint64_t r = 1;
        while (r * r < lo) r++;
        for (; r * r < hi; r++)
            s265_acc[(size_t)(r * r - lo)] = (int32_t)s265_square_f(r);
    }

    uint8_t *w = s265_win;
    uint64_t wb = s265_wbase;
    for (size_t i = 0; i < S265_B; i++) {
#if defined(__GNUC__)
        if (i + 24 < S265_B)
            __builtin_prefetch(w + (size_t)((uint64_t)s265_acc[i + 24] - wb));
#endif
        int32_t f = s265_acc[i];
        if (f < 0) f = -f;
        uint64_t k = lo + i;
        unsigned st = (unsigned)w[(size_t)((uint64_t)f - wb)] + 1;
        w[(size_t)(k - wb)] = (uint8_t)st;
        if (st < 256 && !s265_have[st]) { s265_have[st] = true; s265_first[st] = k; }
    }
    s265_done = hi;
}

seq_t a(idx_t n)
{
    if (!s265_init) s265_setup();
    if (n > 255) return 0;
    while (!s265_have[n]) s265_block();
    return (seq_t)s265_first[(size_t)n];
}
