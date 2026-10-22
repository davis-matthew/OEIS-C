/*
 * A353492  a(1) = 0, and for n > 1, a(n) = 1 if the largest proper divisor of
 *          n is of the form 4k+3, otherwise 0.
 * https://oeis.org/A353492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff the largest proper divisor of n is of the form
 *            4k+3, else 0. The largest proper divisor of n > 1 is n / spf(n),
 *            where spf(n) is the smallest prime factor of n. a(1) = 0
 * Range:     u8 n <= 9730, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

static uint64_t _smallest_prime_factor_uint64(uint64_t n)
{
    /* Return the smallest prime factor of n (n >= 2).  For n == 0 or 1
       the function is undefined; callers must avoid such inputs. */
    if (n % 2 == 0) return 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return i;
    return n;   /* n is prime */
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 (and undefined for 0) */

    /* n is an unsigned 64‑bit type (idx_t).  Compute its smallest prime
       factor using trial division up to sqrt(n).  For the target range
       (n <= 10000) this is negligible. */
    uint64_t small_p = _smallest_prime_factor_uint64((uint64_t)n);

    /* The largest proper divisor is n / spf(n).  Check its residue mod 4. */
    uint64_t large_div = n / small_p;

    return ((large_div & 3u) == 3u) ? (seq_t)1 : (seq_t)0;
}
