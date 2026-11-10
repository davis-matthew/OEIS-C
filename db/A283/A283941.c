/*
 * A283941  Interspersion of the signature sequence of sqrt(5).
 * https://oeis.org/A283941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(r,c) = r*c + Sum_{m=1..c-1} floor(m*sqrt(5)) + Sum_{m>=1,
 *            r-1-floor(m*sqrt(5))>0} (r-1-floor(m*sqrt(5))), i.e. the rank of
 *            r + c*sqrt(5) among all i + j*sqrt(5) (i,j >= 1); a(n) reads T
 *            by antidiagonals: in antidiagonal d the p-th term is T(p, d+1-p)
 * Range:     u8 n <= 120, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Validated: standard, pass, 82 terms through n = 1786 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A283941_fl5(unsigned long long m)
{
    /* floor(m*sqrt(5)) by binary search; m*sqrt(5) < 3*m+1 */
    unsigned long long t = 5ULL * m * m;
    unsigned long long lo = 0, hi = 3ULL * m + 1;
    while (lo < hi) {
        unsigned long long mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= t) lo = mid; else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    unsigned long long N = (unsigned long long)n;
    unsigned long long d = 1;
    while (d * (d + 1) / 2 < N) d++;
    unsigned long long p = N - d * (d - 1) / 2;
    unsigned long long r = p;
    unsigned long long c = d + 1 - p;
    seq_t total = (seq_t)r;
    unsigned long long m;
    for (m = 1; m < c; m++)
        total += (seq_t)(r + A283941_fl5(m));
    for (m = 1;; m++) {
        unsigned long long f = A283941_fl5(m);
        if (r < f + 2) break;
        total += (seq_t)(r - 1 - f);
    }
    return total;
}
