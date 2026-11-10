/*
 * A358631  Irregular triangle T(n, k), n >= 0, k > 0, read by rows of
 *          extended (due to binary expansion of n) Stirling numbers of the
 *          first kind.
 * https://oeis.org/A358631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   T(n,k) = P(n, wt(n)-k+3), 0<k<=wt(n)+2, wt=A000120; P(n,1)=1
 *            (n>0), P(0,1)=P(0,2)=1; P(n,k) =
 *            (wt(q)+2)*P(q,k-1)*(A290255(n)+1) + P(s(q),k), q=A053645(n),
 *            s(m)=m+[A063250(m)>0]*2^(A063250(m)-1). Flattened row by row
 *            (row n has A000120(n)+2 terms); computed by DP over rows
 * Range:     u8 n <= 58, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static unsigned T358631_wt(idx_t n)
{
    unsigned c = 0;
    while (n) { c += (unsigned)(n & 1); n >>= 1; }
    return c;
}

static unsigned T358631_msb(idx_t x)
{
    unsigned r = 0;
    while (x > 1) { x >>= 1; ++r; }
    return r;
}

static unsigned T358631_zeros(idx_t n) /* A290255, n >= 1 */
{
    unsigned msb = T358631_msb(n);
    idx_t rem = n - ((idx_t)1 << msb);
    if (rem == 0) return msb;
    return msb - T358631_msb(rem) - 1;
}

static unsigned T358631_rot(idx_t n) /* A063250 */
{
    unsigned res = 0;
    while (((n + 1) & n) != 0) { ++res; n >>= 1; }
    return res;
}

seq_t a(idx_t n)
{
    /* locate row r and position within the row */
    idx_t r = 0;
    idx_t off = 0;
    for (;;) {
        idx_t len = (idx_t)T358631_wt(r) + 2;
        if (n < off + len) break;
        off += len;
        ++r;
    }
    unsigned k = (unsigned)(n - off) + 1;
    unsigned j = T358631_wt(r) - k + 3;

    size_t W = (size_t)T358631_msb(r > 0 ? r : 1) + 5;
    size_t rows = (size_t)r + 1;
    seq_t *P = mem_new(seq_t, rows * W);

    P[0 * W + 1] = 1;
    P[0 * W + 2] = 1;
    for (idx_t m = 1; m <= r; ++m) {
        idx_t q = m - ((idx_t)1 << T358631_msb(m));
        unsigned rr = T358631_rot(q);
        idx_t sq = q;
        if (rr > 0) sq = q + ((idx_t)1 << (rr - 1));
        seq_t mult = (seq_t)(T358631_wt(q) + 2) * (seq_t)(T358631_zeros(m) + 1);
        size_t mi = (size_t)m, qi = (size_t)q, si = (size_t)sq;
        P[mi * W + 1] = 1;
        for (size_t jj = 2; jj < W; ++jj) {
            P[mi * W + jj] = mult * P[qi * W + jj - 1] + P[si * W + jj];
        }
    }
    seq_t res = P[(size_t)r * W + j];
    mem_free(P);
    return res;
}
