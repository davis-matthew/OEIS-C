/*
 * A306289  The smallest prime factor of numbers greater than 1 and coprime to
 *          6.
 * https://oeis.org/A306289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime factor of the n‑th integer >1 that is
 *            congruent to 1 or 5 modulo 6
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the least prime factor of a uint64_t number.  For the sizes
   we need (< 3·10⁴) simple trial division is fast enough. */
static inline uint64_t spf_uint64(uint64_t n)
{
    if (n <= 1) return n;
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (uint64_t p = 5; p * p <= n; p += 6)
    {
        if (n % p == 0)   return p;
        if (n % (p + 2) == 0) return p + 2;
    }
    return n; /* n itself is prime */
}

seq_t a(idx_t n)
{
    /* n is at least 1.  Count how many admissible numbers we have seen. */
    idx_t seen = 0;
    uint64_t x = 5;               /* first integer >1 that is ≡1 or 5 (mod 6) */

    while (1)
    {
        /* Is x coprime to 6?  (x mod 6 == 1 or 5) */
        if (x % 6 == 1 || x % 6 == 5)
        {
            seen++;
            if (seen == n)
            {
                uint64_t spf = spf_uint64(x);
                return (seq_t)spf;
            }
        }
        x++;
    }
}
