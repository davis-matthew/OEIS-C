/*
 * A308699  Smallest m >= n such that 1 - m! / ((m-n)!*m^n) < 1/2.
 * https://oeis.org/A308699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m >= n such that * 2 * ∏_{k=0}^{n-1} (m-k) > m^n
 *            * * The test is performed using double‐precision logarithms to
 *            avoid * overflow. For integer builds up to 64 bits the
 *            logarithms are * sufficient to determine the correct m, and the
 *            final result is * returned as seq_t. No dynamic al...
 * Range:     u8 n <= 19, u16 n <= 301, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 69 terms through n = 9901 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>

/* Return true iff 2*∏(m-k) > m^n for given n and m.  Uses log to avoid overflow. */
static bool holds_log(idx_t n, idx_t m)
{
    if (n == 0) return true;          /* vacuously true for n = 0 */
    double log_left = log(2.0);       /* log(2) + Σ log(m-k) */
    for (idx_t k = 0; k < n; ++k)
        log_left += log((double)(m - k));
    double log_right = (double)n * log((double)m); /* n*log(m) */
    return log_left > log_right;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* If the smallest possible m already satisfies the condition. */
    if (holds_log(n, n))
        return (seq_t)n;

    /* Find an upper bound by exponential search. */
    idx_t lo = n, hi = n + 1;
    while (!holds_log(n, hi))
        hi *= 2;

    /* Binary search for the smallest m in (lo, hi] with holds_log true. */
    while (lo + 1 < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (holds_log(n, mid))
            hi = mid;
        else
            lo = mid;
    }
    return (seq_t)hi;
}
