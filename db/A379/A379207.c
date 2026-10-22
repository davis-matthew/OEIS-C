/*
 * A379207  a(n) = 2*prime(n+1) - (prime(n+1) - prime(n))^2.
 * https://oeis.org/A379207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000040(n+1) - (A000040(n+1)-A000040(n))^2
 * Range:     u8 n <= 32, u16 n <= 3513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1; return 0 */

    /* Retrieve the (n+1)th and nth primes */
    seq_t p_next = (seq_t)get_prime(n + 1);
    seq_t p_curr = (seq_t)get_prime(n);

    seq_t diff = p_next - p_curr;           /* prime(n+1) - prime(n) */
    seq_t result = (2 * p_next) - (diff * diff);  /* 2*prime(n+1) - diff^2 */

    return result;
}
