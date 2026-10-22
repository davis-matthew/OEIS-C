/*
 * A338274  Number of decompositions of 2*n >= 4 into an unordered sum of two
 *          primes, n - d and n + d, such that d < sqrt(2*n - 1).
 * https://oeis.org/A338274
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of unordered prime pairs (p,q) with p+q=2*n and d =
 *            (q‑p)/2 satisfying d*d < 2*n-1
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Small deterministic primality test for numbers that fit in idx_t
   (sufficient for the range needed here). */
static inline bool is_prime_u64(idx_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* sequence offset starts at 2 */

    idx_t limit = 2 * n - 1;                  /* need d*d < limit */
    seq_t count = 0;

    for (idx_t d = 0; d * d < limit; ++d) {
        idx_t p = n - d;
        idx_t q = n + d;
        if (p < 2) continue;                 /* p must be prime and >= 2 */
        if (is_prime_u64(p) && is_prime_u64(q))
            count = count + 1;
    }
    return count;
}
