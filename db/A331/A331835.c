/*
 * A331835  Replace 2^k in binary expansion of n with k-th prime number for
 *          any k > 0 (and keep 2^0).
 * https://oeis.org/A331835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k>=0} [n has bit k set] * (k==0 ? 1 : prime(k))
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Replace 2^k in the binary expansion of n with the k-th prime for k>0 (keep 2^0). */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t bit = 0;
    while (n)
    {
        if (n & 1)
        {
            if (bit == 0)
                result += (seq_t)1;
            else
                result += (seq_t)get_prime(bit);
        }
        n >>= 1;
        ++bit;
    }
    return result;
}
