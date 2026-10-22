/*
 * A327721  Dimension of quantum lens space needed for non-uniqueness.
 * https://oeis.org/A327721
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented:
 *   a(n) = 4                if n%4==0 and n%3==0
 *   a(n) = 6                if n%4==0 and n%3!=0
 *   a(n) = p+1              where p is the smallest odd prime dividing n
 *                          (or dividing n/2 when n is even but not divisible by 4)
 *   Sequence is defined for n >= 3; values for smaller n are not specified.
 */

#include <stdint.h>

/* Minimal helper to find the smallest odd prime factor of n (n >= 3).
 * For even n not divisible by 4, n is passed as n/2 which is odd. */
static uint64_t smallest_odd_prime_factor(uint64_t n)
{
    if (n <= 1) return 0;            /* Should not occur for valid calls */
    if (n % 3 == 0) return 3;        /* early check */
    if (n % 5 == 0) return 5;        /* optional 5 check */

    /* trial division for odd n */
    for (uint64_t i = 7; i * i <= n; i += 2) {
        if (n % i == 0) return i;
    }
    /* n itself is prime */
    return n;
}

seq_t a(idx_t n)
{
    /* n is unsigned 64‑bit; sequence defined for n >= 3 */
    if (n < 3) return 0;  /* Undefined for smaller indices, return 0 */

    /* Case 1: n is divisible by 4 */
    if ((n & 3) == 0) {          /* n % 4 == 0 */
        if ((n % 3) == 0)        /* n % 3 == 0 */
            return (seq_t)4;
        else
            return (seq_t)6;
    }

    /* Case 2: n is not divisible by 4 */
    uint64_t m;
    if ((n & 1) == 0)            /* even but not multiple of 4: n = 2 * m, m odd */
        m = n >> 1;
    else                         /* odd */
        m = n;

    uint64_t p = smallest_odd_prime_factor(m);   /* p is odd prime */
    return (seq_t)p + 1;
}
