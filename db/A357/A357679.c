/*
 * A357679  a(n) = prime(n)*(prime(n-1) + prime(n+1)).
 * https://oeis.org/A357679
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)*(prime(n-1)+prime(n+1))
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Undefined for n < 2 per OEIS offset 2 */
        return (seq_t)0;
    }
    /* Retrieve the required primes (prime(1)=2) */
    uint32_t p_n   = get_prime(n);
    uint32_t p_nm1 = get_prime(n - 1);
    uint32_t p_np1 = get_prime(n + 1);

    /* Compute the sequence value in seq_t, using only integer ops */
    return (seq_t)p_n * ((seq_t)p_nm1 + (seq_t)p_np1);
}
