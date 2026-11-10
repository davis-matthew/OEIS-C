/*
 * A285332  a(0) = 1, a(1) = 2, a(2n) = A019565(a(n)), a(2n+1) =
 *          A065642(a(n)).
 * https://oeis.org/A285332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1, a(1) = 2, a(2n) = A019565(a(n)), a(2n+1) =
 *            A065642(a(n))
 * Range:     u8 n <= 21, u16 n <= 43, u32 n <= 87, u64 n <= 127,
 *            u128 n <= 175, big any n
 * Validated: standard, pass, 87 terms through n = 684 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static uint32_t *s285332_pl = NULL;
static size_t s285332_pc = 0;
static size_t s285332_lim = 0;

/* make sure at least k+1 primes are available */
static void s285332_ensure(size_t k)
{
    while (s285332_pc <= k) {
        size_t lim = s285332_lim ? s285332_lim * 2 : 2048;
        unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
        size_t cnt = 0, i, j;
        for (i = 2; i <= lim; i++) {
            if (!comp[i]) {
                cnt++;
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
            }
        }
        free(s285332_pl);
        s285332_pl = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
        cnt = 0;
        for (i = 2; i <= lim; i++)
            if (!comp[i]) s285332_pl[cnt++] = (uint32_t)i;
        free(comp);
        s285332_pc = cnt;
        s285332_lim = lim;
    }
}

/* A019565: product of prime(k+1) over set bits k of m */
static seq_t s285332_A019565(seq_t m)
{
    seq_t res = 1;
    size_t bit = 0;
    while (m != 0) {
        if ((m & 1) != 0) {
            s285332_ensure(bit);
            res *= (seq_t)s285332_pl[bit];
        }
        m >>= 1;
        bit++;
    }
    return res;
}

/* A065642: least k > m with same set of distinct primes as m */
static seq_t s285332_A065642(seq_t m)
{
    if (m <= 1) return m;
    size_t cap = 16, cnt = 0, i = 0;
    uint32_t *ps = (uint32_t *)malloc(cap * sizeof(uint32_t));
    seq_t t = m, r = 1;
    while (t != 1) {
        s285332_ensure(i);
        uint32_t p = s285332_pl[i++];
        if (t % p == 0) {
            if (cnt == cap) {
                cap *= 2;
                ps = (uint32_t *)realloc(ps, cap * sizeof(uint32_t));
            }
            ps[cnt++] = p;
            r *= p;
            while (t % p == 0) t /= p;
        }
    }
    seq_t cand = (m / r + 1) * r;
    for (;; cand += r) {
        seq_t u = cand;
        for (size_t k = 0; k < cnt; k++) {
            while (u % ps[k] == 0) u /= ps[k];
        }
        if (u == 1) break;
    }
    free(ps);
    return cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    int top = 63;
    while (((n >> top) & 1) == 0) top--;
    seq_t v = 2;
    for (int b = top - 1; b >= 0; b--) {
        if ((n >> b) & 1)
            v = s285332_A065642(v);
        else
            v = s285332_A019565(v);
    }
    return v;
}
