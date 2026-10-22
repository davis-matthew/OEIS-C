/*
 * A374146  a(n) = prime(g) mod g where prime gap g = prime(n+1) - prime(n).
 * https://oeis.org/A374146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(n+1)-prime(n)) mod (prime(n+1)-prime(n))
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset 1; undefined for n<1 but guard against it */

    /* Obtain the n-th and (n+1)-th primes */
    uint32_t p_n   = get_prime(n);
    uint32_t p_np1 = get_prime(n + 1);

    /* Prime gap */
    uint32_t g = p_np1 - p_n;     /* g >= 1 */

    /* The g-th prime */
    uint32_t p_g = get_prime(g);

    /* Compute result as seq_t */
    return (seq_t)p_g % (seq_t)g;
}
