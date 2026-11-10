/*
 * A365422  Numbers k for which k and A163511(k) have the same prime
 *          signature.
 * https://oeis.org/A365422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that k and A163511(k) have the same
 *            prime signature (search; A163511 exponents are read from the
 *            binary gaps of k)
 * Range:     u8 n <= 35, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void sort_small(int *v, int c) {
    for (int i = 1; i < c; i++) {
        int x = v[i], j = i - 1;
        while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
        v[j + 1] = x;
    }
}

static int A365422_test(uint64_t k) {
    int e1[70], c1 = 0;
    int e2[70], c2 = 0;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            e1[c1++] = e;
        }
    }
    if (m > 1) e1[c1++] = 1;

    /* exponents of A163511(k) */
    int r = 0;
    uint64_t t = k;
    int prev = 0, bp = 1;
    int ex[70];
    while (t) {
        if (t & 1) {
            ex[r++] = bp - prev - 1;
            prev = bp;
        }
        bp++;
        t >>= 1;
    }
    ex[r - 1] += 1;
    for (int i = 0; i < r; i++) if (ex[i] > 0) e2[c2++] = ex[i];

    if (c1 != c2) return 0;
    sort_small(e1, c1);
    sort_small(e2, c2);
    for (int i = 0; i < c1; i++) if (e1[i] != e2[i]) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, k = 0;
    while (cnt < (uint64_t)n) {
        k++;
        if (A365422_test(k)) cnt++;
    }
    return (seq_t)k;
}
