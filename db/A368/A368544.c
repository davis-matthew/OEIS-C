/*
 * A368544  The number of divisors of n whose prime factors are all of the
 *          form k^2+1.
 * https://oeis.org/A368544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏ₚⁿ (eₚ+1) where the product runs over primes p^eₚ‖n, and
 *            p is counted only if p−1 is a perfect square (i.e., p = k²+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns the greatest r such that r·r ≤ x. */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= ( __int128 )x) { ans = mid; lo = mid + 1; }
        else { if (mid==0) break; hi = mid - 1; }
    }
    return ans;
}

/* Returns true iff the prime p is of the form k²+1. */
static inline bool is_special(uint64_t p)
{
    if (p < 2) return false;
    uint64_t sq = isqrt_uint64(p - 1);
    return (__int128)sq * sq == ( __int128 )(p - 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* undefined offset, return 1 for safety */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (is_special(p))
                result *= (seq_t)(e + 1);
        }
    }
    if (m > 1) {              /* remaining prime factor */
        if (is_special(m))
            result *= (seq_t)2; /* e = 1, so e+1 = 2 */
    }
    return result;
}
