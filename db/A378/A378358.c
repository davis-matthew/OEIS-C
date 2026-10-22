/*
 * A378358  Least non-perfect-power >= n.
 * https://oeis.org/A378358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least non‑perfect‑power >= n */
#include <stdint.h>
#include <stdbool.h>

/* compute base^exp, stopping if the result would exceed limit;
   if it would exceed, return limit+1 */
static inline uint64_t pow_limit(uint64_t base, int exp, uint64_t limit)
{
    uint64_t result = 1;
    for (int i = 0; i < exp; ++i) {
        if (result > limit / base)   /* would overflow limit */
            return limit + 1;
        result *= base;
    }
    return result;
}

/* integer kth root of n (floor of n^(1/k)) */
static inline uint64_t integer_root(uint64_t n, int k)
{
    if (k == 1) return n;
    uint64_t lo = 1, hi = n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        uint64_t p = pow_limit(mid, k, n);
        if (p <= n) lo = mid;
        else        hi = mid - 1;
    }
    return lo;
}

/* true if n is a perfect power (including 1) */
static inline bool is_perfect_power_uint64(uint64_t n)
{
    if (n == 1) return true;
    for (int exp = 2; exp <= 64; ++exp) {
        uint64_t r = integer_root(n, exp);
        if (r < 2) break;          /* further exponents cannot succeed */
        uint64_t p = pow_limit(r, exp, n);
        if (p == n) return true;
    }
    return false;
}

/* sequence function: a(n) = least non‑perfect‑power >= n */
seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    while (true) {
        if (!is_perfect_power_uint64(x))
            return (seq_t)x;
        ++x;
    }
}
