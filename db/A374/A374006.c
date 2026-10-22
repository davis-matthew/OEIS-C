/*
 * A374006  Product of the first n primes greater than or equal to n.
 * https://oeis.org/A374006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of the first n primes >= n * a(n) =
 *            Π_{i=π(n‑1)+1}^{n+π(n‑1)} p_i * a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   get_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prime_pi.h"  /* returns seq_t prime_pi(idx_t limit) */
#include "../utils/get_prime.h" /* returns uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* a(0) is the empty product */
    if (n == 0)
        return (seq_t)1;

    /* π(n-1) */
    seq_t pi_seq = prime_pi(n - (idx_t)1);
    idx_t  pi_n_minus1 = (idx_t)pi_seq;

    /* indices of the primes to multiply */
    idx_t start = pi_n_minus1 + (idx_t)1;
    idx_t end   = n + pi_n_minus1;

    seq_t prod = (seq_t)1;
    for (idx_t i = start; i <= end; ++i) {
        uint32_t p = get_prime(i);
        prod *= (seq_t)p;
    }
    return prod;
}
