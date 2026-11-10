/*
 * A248827  Row sums of A187783 and A089759.
 * https://oeis.org/A248827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0..n} T(m,n-m), where T(m,k) = (m*k)!/(k!)^m =
 *            Prod_{i=1..m} C(i*k,k) (antidiagonal sums of A187783)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A248827_binom(idx_t N, idx_t K)
{
    if (K == 0 || K == N) return 1;
    idx_t k = K;
    if (k > N - k) k = N - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(N - k + i) / (seq_t)i;
    }
    return res;
}

static seq_t A248827_T(idx_t m, idx_t k)
{
    if (m == 0 || k == 0) return 1;
    seq_t r = 1;
    for (idx_t i = 1; i <= m; ++i) {
        r = r * A248827_binom(i * k, k);
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t m = 0; m <= n; ++m) {
        s += A248827_T(m, n - m);
    }
    return s;
}
