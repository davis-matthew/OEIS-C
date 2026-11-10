/*
 * A336042  Numbers k such that A000055(k) is divisible by k.
 * https://oeis.org/A336042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k >= 1 such that A000055(k) == 0 (mod k), where
 *            A000055(k) = A000081(k) - Sum_{1<=i<=j, i+j=k}
 *            A000081(i)*A000081(j) + [k even]*binomial(A000081(k/2)+1,2);
 *            computed by a bounded search (k <= 1100, exact bignum A000081
 *            via r(m+1) = (1/m)*Sum_{j=1..m} s(j)*r(m+1-j), s(j) = Sum_{d|j}
 *            d*r...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define T55_LIM 1100
#define T55_L 64

static uint32_t t55_r[T55_LIM + 2][T55_L];
static uint32_t t55_s[T55_LIM + 2][T55_L];
static int t55_rl[T55_LIM + 2], t55_sl[T55_LIM + 2];
static unsigned t55_terms[64];
static int t55_nterms = 0;
static int t55_done = 0;

static int t55_trim(const uint32_t *a, int l)
{
    while (l > 0 && a[l - 1] == 0) l--;
    return l;
}

static void t55_mulacc(uint32_t *acc, const uint32_t *a, int la, const uint32_t *b, int lb)
{
    int i, j;
    for (i = 0; i < la; i++) {
        uint64_t carry = 0;
        uint64_t x = a[i];
        if (x == 0) continue;
        for (j = 0; j < lb; j++) {
            uint64_t t = x * b[j] + acc[i + j] + carry;
            acc[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        j = i + lb;
        while (carry && j < T55_L) {
            uint64_t t = (uint64_t)acc[j] + carry;
            acc[j] = (uint32_t)t;
            carry = t >> 32;
            j++;
        }
    }
}

static void t55_divs(uint32_t *a, int l, uint32_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = l - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static uint32_t t55_mod(const uint32_t *a, int l, uint32_t m)
{
    uint64_t rem = 0;
    int i;
    for (i = l - 1; i >= 0; i--)
        rem = ((rem << 32) | a[i]) % m;
    return (uint32_t)rem;
}

static void t55_init(void)
{
    int m, k, d;
    static uint32_t acc[T55_L];
    static uint32_t tmp[T55_L];
    static uint32_t res[T55_LIM + 2];
    if (t55_done) return;
    t55_done = 1;
    memset(t55_r, 0, sizeof t55_r);
    memset(t55_s, 0, sizeof t55_s);
    t55_r[1][0] = 1; t55_rl[1] = 1;
    t55_s[1][0] = 1; t55_sl[1] = 1;
    for (m = 1; m < T55_LIM; m++) {
        int la;
        memset(acc, 0, sizeof acc);
        for (k = 1; k <= m; k++)
            t55_mulacc(acc, t55_s[k], t55_sl[k], t55_r[m + 1 - k], t55_rl[m + 1 - k]);
        la = t55_trim(acc, T55_L);
        t55_divs(acc, la, (uint32_t)m);
        memcpy(t55_r[m + 1], acc, sizeof acc);
        t55_rl[m + 1] = t55_trim(acc, T55_L);
        memset(acc, 0, sizeof acc);
        for (d = 1; d <= m + 1; d++) {
            if ((m + 1) % d == 0) {
                uint32_t dd = (uint32_t)d;
                uint32_t dv[1];
                dv[0] = dd;
                memset(tmp, 0, sizeof tmp);
                t55_mulacc(tmp, t55_r[d], t55_rl[d], dv, 1);
                {
                    uint64_t c = 0;
                    int i;
                    for (i = 0; i < T55_L; i++) {
                        c += (uint64_t)acc[i] + tmp[i];
                        acc[i] = (uint32_t)c;
                        c >>= 32;
                    }
                }
            }
        }
        memcpy(t55_s[m + 1], acc, sizeof acc);
        t55_sl[m + 1] = t55_trim(acc, T55_L);
    }
    for (k = 1; k <= T55_LIM; k++) {
        uint64_t M = 4ULL * (uint64_t)k;
        uint64_t a;
        int i;
        for (i = 1; i <= k; i++)
            res[i] = t55_mod(t55_r[i], t55_rl[i], (uint32_t)M);
        a = res[k] % M;
        for (i = 1; 2 * i <= k; i++) {
            uint64_t p = ((uint64_t)res[i] * res[k - i]) % M;
            a = (a + M - p) % M;
        }
        if (k % 2 == 0) {
            uint64_t h = res[k / 2];
            uint64_t t = (h * (h + 1) / 2) % M;
            a = (a + t) % M;
        }
        if (a % (uint64_t)k == 0) {
            if (t55_nterms < 64) t55_terms[t55_nterms++] = (unsigned)k;
        }
    }
}

seq_t a(idx_t n)
{
    t55_init();
    if (n < 1 || n > (idx_t)t55_nterms) return 0;
    return (seq_t)t55_terms[n - 1];
}
