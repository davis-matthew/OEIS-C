/*
 * A374853  Obverse convolution (n)**(Prime(n)); see Comments.
 * https://oeis.org/A374853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} (k + Prime(n-k+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product is taken over k = 1..n of the sum of k and the (n-k+1)-th prime. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* prime(n-k+1) gives the (n-k+1)-th prime */
        uint32_t p = get_prime(n - k + 1);
        seq_t term = (seq_t)k + (seq_t)p;
        result *= term;
    }
    return result;
}
