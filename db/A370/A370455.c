/*
 * A370455  a(n) = greatest m such that 2^m divides prime(n+1)*prime(n+2) -
 *          prime(n)*prime(n+3).
 * https://oeis.org/A370455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2-adic valuation of (prime(n+1)*prime(n+2) -
 *            prime(n)*prime(n+3))
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* primes required: p_n, p_{n+1}, p_{n+2}, p_{n+3} */
    seq_t p1 = (seq_t) get_prime(n);
    seq_t p2 = (seq_t) get_prime(n + 1);
    seq_t p3 = (seq_t) get_prime(n + 2);
    seq_t p4 = (seq_t) get_prime(n + 3);

    seq_t val = p2 * p3 - p1 * p4;
    /* take absolute value (the 2-adic valuation is the same for -val) */
    if (val < 0)
        val = -val;

    idx_t m = 0;
    /* count trailing zeros of val */
    while ((val & 1) == 0)
    {
        val >>= 1;
        m++;
    }

    return (seq_t) m;
}
