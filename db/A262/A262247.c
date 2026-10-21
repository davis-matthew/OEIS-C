/*
 * A262247  Number of squares formed from a square composed of p^2 unit
 *          squares where p is n-th prime.
 * https://oeis.org/A262247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * (prime(n)+1) * (2*prime(n)+1) / 6
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th prime (p). */
    seq_t p = (seq_t)get_prime(n);

    /* Prepare the three factors. */
    seq_t a1 = p;
    seq_t b1 = p + 1;
    seq_t c1 = (seq_t)2 * p + 1;

    /* Divide by 2 (exactly one of a1 or b1 is even). */
    if (a1 % (seq_t)2 == 0)
        a1 /= (seq_t)2;
    else
        b1 /= (seq_t)2;  /* b1 is even in this case */

    /* Divide by 3 (at least one factor is divisible by 3). */
    if (a1 % (seq_t)3 == 0)
        a1 /= (seq_t)3;
    else if (b1 % (seq_t)3 == 0)
        b1 /= (seq_t)3;
    else
        c1 /= (seq_t)3;  /* c1 is divisible by 3 in this case */

    /* Multiply the reduced factors to get the result. */
    seq_t res = a1;
    res *= b1;
    res *= c1;
    return res;
}
