/*
 * A379817  Irregular triangle T(n, k), n >= 0, k >= 0, read by rows such that
 *          T(n,k) = f(n,k)/f(2^k-1,k) where f(n,k) is defined in Comments.
 * https://oeis.org/A379817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   T(n,k) = c(n,wt(n)-k), wt = A000120, c(n,0)=A341392(n), c(0,k)=0
 *            for k>0, c(2n+1,k) = c(n,k) + (wt(n)-k+2)*c(n,k-1), c(2n,k) =
 *            (wt(n)-k+1)*c(2n+1,k) for n>0; row n has wt(n)+1 entries; a(idx)
 *            is the flattened triangle
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned T_wt(idx_t x) {
    unsigned c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
}

seq_t a(idx_t idx) {
    /* locate row n and column k */
    idx_t n = 0;
    for (;;) {
        idx_t len = (idx_t)T_wt(n) + 1;
        if (idx < len) break;
        idx -= len;
        n++;
    }
    idx_t k = idx;

    /* build c(n, .) from the top bit down */
    seq_t v[72], u[72];
    unsigned w = 0;
    v[0] = (seq_t)1;
    int started = 0;
    for (int bit = 63; bit >= 0; --bit) {
        int b = (int)((n >> bit) & 1);
        if (!started) {
            if (!b) continue;
            started = 1;
        }
        /* u = c(2p+1, .) from v = c(p, .) */
        for (unsigned j = 0; j <= w + 1; ++j) {
            seq_t val = (j <= w) ? v[j] : (seq_t)0;
            if (j > 0)
                val += (seq_t)(w - j + 2) * v[j - 1];
            u[j] = val;
        }
        if (b) {
            for (unsigned j = 0; j <= w + 1; ++j) v[j] = u[j];
            w++;
        } else {
            if (w == 0 && v[0] == (seq_t)1 && n == 0) { /* not reached */ }
            for (unsigned j = 0; j <= w; ++j)
                v[j] = (seq_t)(w - j + 1) * u[j];
        }
    }
    /* here w == wt(n) */
    return v[w - (unsigned)k];
}
