/*
 * A248139  Least positive integer m such that m + n divides f(m) + f(n),
 *          where f(.) is given by A000172.
 * https://oeis.org/A248139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for the least m >= 1 with (m+n) | f(m) + f(n), where f =
 *            A000172 (Franel numbers) is generated exactly by (k+1)^2*f(k+1)
 *            = (7*k^2+7*k+2)*f(k) + 8*k^2*f(k-1), f(0) = 1, f(1) = 2, and
 *            reduced mod m+n at each step
 * Range:     u8 n <= 31, u16 n <= 4355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9983 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned __int128 fr_u128;

static uint64_t *fr_P = 0, *fr_F = 0, *fr_T = 0, *fr_N = 0;
static size_t fr_cap = 0, fr_lP = 0, fr_lF = 0, fr_ncap = 0, fr_lN = 0;

static void fr_grow(size_t need)
{
    size_t nc;
    if (need <= fr_cap) return;
    nc = need * 2 + 16;
    fr_P = (uint64_t *)realloc(fr_P, nc * sizeof(uint64_t));
    fr_F = (uint64_t *)realloc(fr_F, nc * sizeof(uint64_t));
    fr_T = (uint64_t *)realloc(fr_T, nc * sizeof(uint64_t));
    fr_cap = nc;
}

static void fr_reset(void)
{
    fr_grow(4);
    fr_P[0] = 1; fr_lP = 1;   /* f(0) */
    fr_F[0] = 2; fr_lF = 1;   /* f(1) */
}

/* from P = f(k-1), F = f(k) compute f(k+1); afterwards P = f(k), F = f(k+1) */
static void fr_step(uint64_t k)
{
    uint64_t c1 = 7 * k * k + 7 * k + 2, c2 = 8 * k * k, carry = 0;
    uint64_t d = k + 1, dd, inv, c;
    size_t i, len;
    unsigned s, tz = 0;
    uint64_t *tmp;
    fr_grow(fr_lF + 3);
    for (i = 0; i < fr_lF; i++) {
        fr_u128 acc = (fr_u128)fr_F[i] * c1 + carry;
        if (i < fr_lP) acc += (fr_u128)fr_P[i] * c2;
        fr_T[i] = (uint64_t)acc;
        carry = (uint64_t)(acc >> 64);
    }
    len = fr_lF;
    if (carry) fr_T[len++] = carry;
    fr_T[len] = 0;
    while ((d & 1) == 0) { d >>= 1; tz++; }
    s = 2 * tz;
    if (s) {
        for (i = 0; i < len; i++)
            fr_T[i] = (fr_T[i] >> s) | (fr_T[i + 1] << (64 - s));
        while (len > 1 && fr_T[len - 1] == 0) len--;
    }
    dd = d * d;
    if (dd > 1) {
        int it;
        inv = dd;
        for (it = 0; it < 6; it++) inv *= 2 - dd * inv;
        c = 0;
        for (i = 0; i < len; i++) {
            uint64_t sv = fr_T[i], l = sv - c, q;
            c = (l > sv);
            q = l * inv;
            fr_T[i] = q;
            c += (uint64_t)(((fr_u128)q * dd) >> 64);
        }
        while (len > 1 && fr_T[len - 1] == 0) len--;
    }
    tmp = fr_P; fr_P = fr_F; fr_F = fr_T; fr_T = tmp;
    fr_lP = fr_lF; fr_lF = len;
}

static uint64_t fr_red(uint64_t y, uint64_t M, uint64_t minv)
{
    uint64_t q = (uint64_t)(((fr_u128)y * minv) >> 64);
    uint64_t r = y - q * M;
    while (r >= M) r -= M;
    return r;
}

/* bignum x (64-bit limbs) mod M, M < 2^32 */
static uint64_t fr_mod(const uint64_t *x, size_t len, uint64_t M)
{
    uint64_t minv = UINT64_MAX / M, r = 0;
    size_t i;
    for (i = len; i-- > 0;) {
        r = fr_red((r << 32) | (x[i] >> 32), M, minv);
        r = fr_red((r << 32) | (x[i] & 0xFFFFFFFFull), M, minv);
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n, k, m;
    size_t i;
    fr_reset();
    if (N == 0) {
        fr_N = (uint64_t *)realloc(fr_N, 4 * sizeof(uint64_t));
        fr_N[0] = 1; fr_lN = 1;
    } else {
        for (k = 1; k < N; k++) fr_step(k);
        if (fr_lF > fr_ncap) {
            fr_ncap = fr_lF * 2 + 4;
            fr_N = (uint64_t *)realloc(fr_N, fr_ncap * sizeof(uint64_t));
        }
        for (i = 0; i < fr_lF; i++) fr_N[i] = fr_F[i];
        fr_lN = fr_lF;
    }
    fr_reset();
    for (m = 1;; m++) {
        uint64_t M = m + N;
        uint64_t r = fr_mod(fr_F, fr_lF, M) + fr_mod(fr_N, fr_lN, M);
        if (r % M == 0)
            return (seq_t)m;
        fr_step(m);
    }
}
