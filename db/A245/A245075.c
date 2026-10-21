/*
 * A245075  Difference between least prime factor of n and the digital root of
 *          n.
 * https://oeis.org/A245075
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign, base
 * Formula:   a(n) = least_prime_factor(n) - digital_root(n)
 * Range:     i8 n <= 136, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the digital root of an unsigned 64‑bit integer. */
static inline int digital_root_uint64(uint64_t n)
{
    if (n == 0)
        return 0;
    int r = n % 9;
    return r == 0 ? 9 : r;
}

/* Return the least prime factor of n (n >= 2).  If n is prime,
   the function returns n itself. */
static inline uint64_t least_prime_factor_uint64(uint64_t n)
{
    if (n % 2ULL == 0ULL)
        return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
    {
        if (n % d == 0ULL)
            return d;
    }
    return n;  /* n is prime */
}

/* Required function: compute a(n).  n is a 64‑bit unsigned integer. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  Calls with n < 2 are
       unspecified; we return 0 for them. */
    if (n < 2U)
        return (seq_t)0;

    uint64_t lpf = least_prime_factor_uint64(n);
    int dr = digital_root_uint64(n);
    return (seq_t)lpf - (seq_t)dr;
}
