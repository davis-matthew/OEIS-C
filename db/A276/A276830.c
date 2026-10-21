/*
 * A276830  Number of ways to write n as ((p-1)/2)^2 + P_2, where p is an odd
 *          prime and P_2 is a product of at most two primes.
 * https://oeis.org/A276830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as ((p-1)/2)^2 + P₂, where p is
 *            an odd prime and P₂ is a product of at most two primes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_P2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_P2.h"

/* Integer square‑root for unsigned 64‑bit values. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            ans = 0;
            low = mid + 1;
        } else if (mid > n / mid) {   /* mid*mid > n, avoid overflow */
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* Primality test by trial division up to sqrt(n). */
static bool is_prime_u64(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    idx_t limit = isqrt_u64(n);
    for (idx_t d = 3; d <= limit; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* behavior below the offset is irrelevant */

    seq_t result = 0;
    idx_t kmax = isqrt_u64(n);          /* largest k such that k^2 <= n */

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t p = 2 * k + 1;            /* odd candidate for prime p */
        if (!is_prime_u64(p)) continue;/* only primes p */

        idx_t sq = k * k;              /* ((p-1)/2)^2 */
        if (sq > n) continue;          /* safety, though loop bound prevents this */

        uint64_t m = (uint64_t)n - (uint64_t)sq; /* remaining part */
        if (m == 0) continue;          /* P₂ cannot be 0 */

        if (is_P2(m)) ++result;        /* count this representation */
    }
    return result;
}
