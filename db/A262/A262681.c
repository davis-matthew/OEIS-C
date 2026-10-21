/*
 * A262681  Odd bisection of A262680.
 * https://oeis.org/A262681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of perfect squares encountered (including the
 *            start) before reaching zero when repeatedly applying k ← k –
 *            d(k), where d(k) is the number of divisors of k. Here the
 *            starting value is k = 2*n + 1 (always odd)
 * Range:     u8 n <= 9493, u16 n <= 9493, u32 n <= 9493, u64 n <= 9493,
 *            u128 n <= 9493, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root for idx_t values.
   Uses a binary‑search style loop that is safe for any idx_t. */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Test whether the idx_t argument is a perfect square. */
static inline bool is_square_idx(idx_t n)
{
    if (n == 0) return true;
    idx_t r = isqrt_idx(n);
    return r * r == n;
}

/* Compute the number of divisors d(n) for an idx_t value n.
   The result fits in idx_t (even for the maximal value that can occur in this
   evaluation). */
static inline idx_t divisor_count_idx(idx_t n)
{
    if (n <= 1) return 1;   /* d(0) isn't needed, d(1)=1. */
    idx_t d = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            d *= (e + 1);
        }
    }
    if (temp > 1) {
        /* Remaining factor is a prime. */
        d *= 2;
    }
    return d;
}

seq_t a(idx_t n)
{
    idx_t k = 2 * n + 1;          /* starting k (always odd) */
    seq_t count = 0;              /* number of squares seen */

    while (k > 0) {
        if (is_square_idx(k)) {
            ++count;              /* count the square */
        }
        idx_t d = divisor_count_idx(k);
        k -= d;                   /* step to the next value */
    }
    return count;
}
