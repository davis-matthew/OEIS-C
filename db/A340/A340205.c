/*
 * A340205  Number of primes in A339443 among the values of A339443(k) for k =
 *          1..n.
 * https://oeis.org/A340205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (pi(c(k)) - pi(c(k)-1)), c = A339443;
 *            computed by generating the terms (s,t) with s+t=k, t<=s, s
 *            decreasing, for k=2,3,... and counting primes by trial division
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 74 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A340205_isp(idx_t x)
{
    if (x < 2) return false;
    for (idx_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    idx_t produced = 0;
    idx_t count = 0;
    for (idx_t k = 2; produced < n; k++) {
        for (idx_t t = 1; t <= k / 2 && produced < n; t++) {
            idx_t s = k - t;
            if (A340205_isp(s)) count++;
            produced++;
            if (produced >= n) break;
            if (A340205_isp(t)) count++;
            produced++;
        }
    }
    return (seq_t)count;
}
