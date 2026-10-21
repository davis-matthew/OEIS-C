/*
 * A291362  The arithmetic function u(n,2,7).
 * https://oeis.org/A291362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 for n = 1; * if n is prime: * return n if n ≤ 7, * else
 *            8; * if n is even composite: return 2; * if n is odd composite:
 *            return the smallest prime factor of n
 * Range:     u8 n <= 9684, u16 n <= 9684, u32 n <= 9684, u64 n <= 9684,
 *            u128 n <= 9684, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)8;

    /* Prime case */
    if (is_prime(n)) {
        if (n <= 7ULL)
            return (seq_t)n;
        else
            return (seq_t)8;
    }

    /* Composite case */
    if ((n & 1ULL) == 0)          /* even composite */
        return (seq_t)2;

    /* odd composite – find smallest prime factor */
    for (idx_t p = 3; p <= n / p; p += 2)
        if (n % p == 0)
            return (seq_t)p;

    /* Fallback – should not happen. */
    return (seq_t)n;
}
