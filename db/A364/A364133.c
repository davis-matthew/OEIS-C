/*
 * A364133  Index k of A007814(A000127(k)) at record terms.
 * https://oeis.org/A364133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k with v2(A000127(k)) > v2(A000127(a(n-1))),
 *            a(0)=1; with A000127(k) = (k^4-6k^3+23k^2-18k+24)/24, found by
 *            2-adic residue lifting of the polynomial
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 oeis_u128_t;

static oeis_u128_t A364133_P(oeis_u128_t k)
{
    return ((((k - 6) * k + 23) * k - 18) * k + 24);
}

static int A364133_v(oeis_u128_t k)
{
    oeis_u128_t p = A364133_P(k);
    int c = 0;
    if (p == 0) return 1000;
    while ((p & 1) == 0) { p >>= 1; c++; }
    return c - 3;
}

/* least k >= 1 with 2^m | P(k) */
static oeis_u128_t A364133_find(int m)
{
    size_t cap = 16, n = 1, i;
    oeis_u128_t *cur = (oeis_u128_t *)malloc(cap * sizeof(oeis_u128_t));
    int j;
    oeis_u128_t best;
    int have = 0;
    cur[0] = 0;
    for (j = 0; j < m; j++) {
        size_t ncap = 16, nn = 0;
        oeis_u128_t *nxt = (oeis_u128_t *)malloc(ncap * sizeof(oeis_u128_t));
        oeis_u128_t mask = (((oeis_u128_t)1) << (j + 1)) - 1;
        oeis_u128_t bit = ((oeis_u128_t)1) << j;
        for (i = 0; i < n; i++) {
            int t;
            for (t = 0; t < 2; t++) {
                oeis_u128_t c = cur[i] + (t ? bit : 0);
                if ((A364133_P(c) & mask) == 0) {
                    if (nn == ncap) {
                        ncap *= 2;
                        nxt = (oeis_u128_t *)realloc(nxt, ncap * sizeof(oeis_u128_t));
                    }
                    nxt[nn++] = c;
                }
            }
        }
        free(cur);
        cur = nxt;
        n = nn;
        cap = ncap;
    }
    best = ((oeis_u128_t)1) << m;
    for (i = 0; i < n; i++) {
        if (cur[i] != 0 && (!have || cur[i] < best)) { best = cur[i]; have = 1; }
    }
    if (!have) best = ((oeis_u128_t)1) << m;
    (void)cap;
    free(cur);
    return best;
}

seq_t a(idx_t n)
{
    static oeis_u128_t rec[256];
    static int cnt = 0;
    if (cnt == 0) { rec[0] = 1; cnt = 1; }
    while ((idx_t)cnt <= n && cnt < 256) {
        oeis_u128_t prev = rec[cnt - 1];
        int v = A364133_v(prev);
        int m = v + 4;
        if (m >= 120) break;
        rec[cnt] = A364133_find(m);
        cnt++;
    }
    if (n >= (idx_t)cnt) return (seq_t)0;
    return (seq_t)rec[n];
}
