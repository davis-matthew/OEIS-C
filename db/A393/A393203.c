/*
 * A393203  a(n) = (1/2)*(prime(n + 2)^2 - prime(n + 1)*prime(n)).
 * https://oeis.org/A393203
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+2)^2 - prime(n+1)*prime(n))/2
 * Range:     u8 n <= 7, u16 n <= 324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;              /* undefined below offset */
    seq_t p_n  = (seq_t)get_prime(n);        /* prime(n)   */
    seq_t p_n1 = (seq_t)get_prime(n + 1);    /* prime(n+1) */
    seq_t p_n2 = (seq_t)get_prime(n + 2);    /* prime(n+2) */

    seq_t val = p_n2 * p_n2 - p_n1 * p_n;    /* numerator part */
    val = val / 2;                          /* divide by 2 */

    return val;
}
