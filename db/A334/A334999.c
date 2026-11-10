/*
 * A334999  Permutation of nonnegative integers that converts A162911 into
 *          A162912.
 * https://oeis.org/A334999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the index m in the same row (2^k <= m < 2^(k+1)) such
 *            that A162911(m) = A162912(n), matching in order: if n is the
 *            j-th index in its row with a given denominator, m is the j-th
 *            index in the row with that numerator
 * Range:     u8 n <= 255, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 79 terms through n = 4063 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void A334999_pair(idx_t i, int k, seq_t *num, seq_t *den)
{
    seq_t N = 1, M = 1, P = 1, Q = 1;
    int b;
    for (b = k - 1; b >= 0; b--) {
        int bit = (int)((i >> b) & 1);
        seq_t nn, mm, pp, qq;
        if (bit == 0) {
            nn = M;     mm = N + M;
            pp = P + Q; qq = P;
        } else {
            nn = N + M; mm = N;
            pp = Q;     qq = P + Q;
        }
        N = nn; M = mm; P = pp; Q = qq;
    }
    *num = N;
    *den = P;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    int k = 63 - __builtin_clzll((unsigned long long)n);
    idx_t lo = ((idx_t)1) << k;
    idx_t hi = (k == 63) ? (idx_t)(~(idx_t)0) : ((lo << 1) - 1);

    seq_t Nn, Dn;
    A334999_pair(n, k, &Nn, &Dn);

    /* rank of n among row elements with the same denominator */
    idx_t rank = 0;
    idx_t i;
    for (i = lo; i < n; i++) {
        seq_t x, d;
        A334999_pair(i, k, &x, &d);
        if (d == Dn) rank++;
    }

    /* find the (rank+1)-th element with numerator Dn */
    idx_t cnt = 0;
    for (i = lo; ; i++) {
        seq_t x, d;
        A334999_pair(i, k, &x, &d);
        if (x == Dn) {
            if (cnt == rank) return (seq_t)i;
            cnt++;
        }
        if (i == hi) break;
    }
    return (seq_t)0;
}
