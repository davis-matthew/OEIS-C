/*
 * A277878  Smallest prime q with q > r such that p = (q + r + 1)/r, where p =
 *          prime(n) and r = A277879(n).
 * https://oeis.org/A277878
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by searching for the smallest prime r such that  
   q = r * (prime(n)-1) - 1 is itself prime.  The returned q is a(n). */

#include <stdbool.h>
#include "../utils/get_prime.h"      // uint32_t get_prime(idx_t k)
#include "../utils/is_prime_big.h"   // bool is_prime_big(seq_t n)

seq_t a(idx_t n)
{
    if (n < 2) return 0;          // offset starts at 2
    /* prime(n) */
    seq_t p = (seq_t)get_prime(n);
    seq_t p_minus1 = p - 1;

    /* iterate over primes r starting from 2 */
    for (idx_t r_index = 1;; ++r_index)
    {
        seq_t r = (seq_t)get_prime(r_index);
        seq_t q = r * p_minus1 - 1;
        if (is_prime_big(q))
            return q;
    }
}
