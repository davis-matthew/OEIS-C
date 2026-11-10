/*
 * A365371  Record values of A365367.
 * https://oeis.org/A365371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A365367(A365370(n)). Each record is found by a 3-adic
 *            search: take the previous record v and find the least m whose
 *            iteration avoids multiples of 3 for v steps. That m is A365370
 *            of the next record, and A365367(m) is the record value
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 A365371_u128;

static uint32_t *A365371_buf = NULL;
static size_t A365371_cap = 0;

static void A365371_ensure(size_t need)
{
    if (need > A365371_cap) {
        size_t c = A365371_cap ? A365371_cap : 16;
        while (c < need) c *= 2;
        A365371_buf = (uint32_t *)realloc(A365371_buf, c * sizeof(uint32_t));
        A365371_cap = c;
    }
}

/* Number of consecutive levels i = 0,1,... (at most maxlev) with y_i not
   divisible by 3, where y_{i+1} = floor((5*y_i+1)/3) = round(5*y_i/3). */
static unsigned A365371_run(A365371_u128 y, unsigned maxlev)
{
    unsigned i = 0;
    const A365371_u128 lim = (~(A365371_u128)0 - 1) / 5;
    size_t len, k;
    uint32_t *l;
    while (i < maxlev && y <= lim) {
        if (y % 3 == 0) return i;
        y = (5 * y + 1) / 3;
        i++;
    }
    if (i >= maxlev) return maxlev;
    A365371_ensure(8);
    l = A365371_buf;
    for (k = 0; k < 4; k++) l[k] = (uint32_t)(y >> (32 * k));
    len = 4;
    while (len > 1 && l[len - 1] == 0) len--;
    while (i < maxlev) {
        uint64_t r = 0, carry = 1, rem = 0;
        for (k = 0; k < len; k++) r += l[k] % 3u;   /* 2^32 == 1 (mod 3) */
        if (r % 3u == 0) return i;
        for (k = 0; k < len; k++) {
            uint64_t t = (uint64_t)l[k] * 5u + carry;
            l[k] = (uint32_t)t;
            carry = t >> 32;
        }
        if (carry) {
            A365371_ensure(len + 2);
            l = A365371_buf;
            l[len++] = (uint32_t)carry;
        }
        for (k = len; k-- > 0;) {
            uint64_t t = (rem << 32) | l[k];
            l[k] = (uint32_t)(t / 3u);
            rem = t % 3u;
        }
        while (len > 1 && l[len - 1] == 0) len--;
        i++;
    }
    return maxlev;
}

static A365371_u128 A365371_best;
static unsigned A365371_need;

/* class r mod M = 3^j passes levels 0..j-1; Y = y_j(r); P = 5^j */
static void A365371_dfs(A365371_u128 r, A365371_u128 M, A365371_u128 Y,
                        A365371_u128 P, unsigned j)
{
    unsigned t;
    if (j == A365371_need) {
        if (r < A365371_best) A365371_best = r;
        return;
    }
    if (r + M >= A365371_best) {
        if (r < A365371_best &&
            A365371_run(Y, A365371_need - j) == A365371_need - j)
            A365371_best = r;
        return;
    }
    for (t = 0; t < 3; t++) {
        A365371_u128 r2 = r + (A365371_u128)t * M;
        A365371_u128 Y2 = Y + (A365371_u128)t * P;
        if (r2 >= A365371_best) break;
        if (Y2 % 3 == 0) continue;
        A365371_dfs(r2, 3 * M, (5 * Y2 + 1) / 3, 5 * P, j + 1);
    }
}

/* least n >= 1 with A365367(n) >= s */
static A365371_u128 A365371_findN(unsigned s, A365371_u128 lo)
{
    A365371_u128 B = 2 * lo + 16;
    const A365371_u128 cap = (A365371_u128)1 << 80;
    if (s <= 1) return 1;
    A365371_need = s - 1;
    while (B <= cap) {
        A365371_best = B;
        A365371_dfs(0, 1, 0, 1, 0);
        if (A365371_best < B) return A365371_best;
        B *= 2;
    }
    return 0;
}

static unsigned *A365371_rec = NULL;
static size_t A365371_cnt = 0, A365371_rcap = 0;
static A365371_u128 A365371_N = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A365371_cnt == 0) {
        A365371_rcap = 64;
        A365371_rec = (unsigned *)malloc(A365371_rcap * sizeof(unsigned));
        A365371_N = 1;
        A365371_rec[0] = A365371_run(1, ~0u) + 1;
        A365371_cnt = 1;
    }
    while ((idx_t)A365371_cnt < n) {
        unsigned s = A365371_rec[A365371_cnt - 1] + 1;
        A365371_u128 N = A365371_findN(s, A365371_N);
        if (N == 0) return (seq_t)0;
        A365371_N = N;
        if (A365371_cnt == A365371_rcap) {
            A365371_rcap *= 2;
            A365371_rec = (unsigned *)realloc(A365371_rec,
                                              A365371_rcap * sizeof(unsigned));
        }
        A365371_rec[A365371_cnt++] = A365371_run(N, ~0u) + 1;
    }
    return (seq_t)A365371_rec[n - 1];
}
