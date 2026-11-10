/*
 * A364802  Smallest number that reaches 1 after n iterations of the map x ->
 *          A356874(x).
 * https://oeis.org/A364802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(0)=1, a(1)=2, a(n) = smallest k with A356874(k) = a(n-1) for
 *            n>=2; k found greedily as the smallest binary number whose set
 *            bits j give distinct Fibonacci numbers F_{j+1} summing to a(n-1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A364802_g(seq_t m) {
    /* smallest k with A356874(k) = m (m >= 2) */
    seq_t F[100];
    F[0] = 0; F[1] = 1; F[2] = 1;
    for (int i = 3; i < 100; i++) F[i] = F[i-1] + F[i-2];
    int t = 1;
    while (t < 90 && F[t+1] - 1 < m) t++;
    seq_t k = 0;
    for (int i = t; i >= 1; i--) {
        if (m <= F[i+1] - 1) continue;
        k |= ((seq_t)1) << (i - 1);
        m -= F[i];
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t v = 2;
    for (idx_t i = 2; i <= n; i++) {
        v = A364802_g(v);
    }
    return v;
}
