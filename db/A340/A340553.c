/*
 * A340553  a(n) = card({k: prime(n) mod k is not prime}).
 * https://oeis.org/A340553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k ∈ [1, prime(n)] such that prime(n) mod k is
 *            not a prime
 * Range:     u8 n <= 54, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* nth prime, fits in 32 bits for the input ranges we consider */
    seq_t p = (seq_t)get_prime(n);

    seq_t count = 0;
    /* iterate k from 1 up to p inclusive */
    for (idx_t k = 1; k <= p; ++k)
    {
        seq_t r = p % k;
        if (!is_prime_small(r))
            ++count;
    }
    return count;
}
