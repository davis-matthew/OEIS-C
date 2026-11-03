/*
 * A259583  Sum of numbers in the n-th antidiagonal of the reciprocity array
 *          of 3.
 * https://oeis.org/A259583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} Sum_{k=0..m-1} floor((n*k + 3)/m)
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 61 terms through n = 428 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute floor_sum(n, m, a, b) = Σ_{i=0}^{n-1} ⌊(a*i + b)/m⌋ */
#include <stdint.h>

static inline unsigned long long floor_sum_ull(unsigned long long n,
                                               unsigned long long m,
                                               unsigned long long a,
                                               unsigned long long b)
{
    unsigned long long ans = 0;
    while (1) {
        if (a >= m) {
            ans += (n - 1) * n * (a / m) / 2;
            a %= m;
        }
        if (b >= m) {
            ans += n * (b / m);
            b %= m;
        }
        unsigned long long y = a * n + b;
        if (y < m) break;
        n = y / m;
        b = y % m;
        unsigned long long tmp = a;
        a = m;
        m = tmp;
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t m = 1; m <= n; ++m) {
        unsigned long long part =
            floor_sum_ull((unsigned long long)m,
                          (unsigned long long)m,
                          (unsigned long long)n,
                          (unsigned long long)3);
        total += (seq_t)part;
    }
    return total;
}
