/*
 * A338530  a(n) = (prime(n) + a(n-1)) mod prime(n-1), a(1) = 1.
 * https://oeis.org/A338530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = (prime(n) + a(n-1)) mod prime(n-1) for n ≥ 2
 * Range:     u8 n <= 43, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term */
    if (n <= 1)  /* n == 1 returns 1, and n == 0 is undefined but we return 0 */
        return (n == 1) ? (seq_t)1 : (seq_t)0;

    seq_t prev = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t p_i     = (seq_t)get_prime(i);       /* prime(n)      */
        seq_t p_prev  = (seq_t)get_prime(i - 1);   /* prime(n-1)    */
        /* Compute (prime(n) + a(n-1)) mod prime(n-1) */
        seq_t sum     = p_i + prev;                /* stays < 2*prime(n) */
        prev = sum % p_prev;
    }
    return prev;
}
