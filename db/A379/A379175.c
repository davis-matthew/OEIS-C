/*
 * A379175  Irregular triangle T(n, k), n >= 0, k =
 *          1..ceiling(2^(A007895(n)-1)); the n-th row lists the nonnegative
 *          integers m such that A184617(m) = A003714(n).
 * https://oeis.org/A379175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   row n lists the m >= 0 with A184617(m) = (m XOR 3m)/2 =
 *            A003714(n), in increasing order. With t = A003714(n) and S = t,
 *            those m are 2*(top bit of t + any subset of its other set bits)
 *            - t; row n has max(1, 2^(A007895(n)-1)) terms
 * Range:     u8 n <= 170, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 86 terms through n = 9861 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Zeckendorf of r: returns fibbinary t (A003714) and number of terms in *k (A007895) */
static uint64_t A379175_zeck(uint64_t r, int *k)
{
    uint64_t f[92];
    int cnt = 2, i;
    uint64_t t = 0;
    *k = 0;
    f[0] = 1;
    f[1] = 2;
    while (cnt < 92 && f[cnt - 1] <= r) {
        f[cnt] = f[cnt - 1] + f[cnt - 2];
        cnt++;
    }
    for (i = cnt - 1; i >= 0; i--) {
        t <<= 1;
        if (f[i] <= r) {
            r -= f[i];
            t |= 1;
            (*k)++;
        }
    }
    return t;
}

seq_t a(idx_t n)
{
    uint64_t cum = 0, r = 0, t, len, p, top, subset, bit;
    int k;
    for (;;) {
        t = A379175_zeck(r, &k);
        len = (k == 0) ? 1 : ((uint64_t)1 << (k - 1));
        if (cum + len > (uint64_t)n) break;
        cum += len;
        r++;
    }
    p = (uint64_t)n - cum;
    if (t == 0) return (seq_t)0;
    top = 1;
    while ((top << 1) <= t && (top << 1) != 0) top <<= 1;
    subset = top;
    for (bit = 1; bit < top; bit <<= 1) {
        if (t & bit) {
            if (p & 1) subset |= bit;
            p >>= 1;
        }
    }
    return (seq_t)subset * 2 - (seq_t)t;
}
