/*
 * A356208  a(n) is the number of occurrences of n in A133388.
 * https://oeis.org/A356208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with A133388(k) = n, i.e. the number of j in
 *            0..n such that n^2+j^2 has no representation p^2+q^2 with p > n
 *            (found by direct search)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A356208_isqrt(idx_t x)
{
    idx_t r = 0, lo = 0, hi = (x >> 1) + 1;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid * mid <= x) { r = mid; lo = mid + 1; }
        else { if (mid == 0) break; hi = mid - 1; }
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t j;
    if (n == 0) return count;
    for (j = 0; j <= n; j++) {
        idx_t k = n * n + j * j;
        idx_t p;
        int ok = 1;
        for (p = n + 1; p * p <= k; p++) {
            idx_t rem = k - p * p;
            idx_t q = A356208_isqrt(rem);
            if (q * q == rem) { ok = 0; break; }
        }
        if (ok) count++;
    }
    return count;
}
