/*
 * A273339  Smallest composite c such that n^(c-1) != 1 (mod c^2), i.e.,
 *          smallest composite c that is not a "Wieferich pseudoprime" to base
 *          n.
 * https://oeis.org/A273339
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite c such that n^(c-1) != 1 (mod c^2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is not defined; return 0 in that case */
    if (n < 2)
        return (seq_t)0;

    for (idx_t c = 4;; ++c)
    {
        /* Skip primes */
        if (is_prime(c))
            continue;

        seq_t modulus = (seq_t)c * (seq_t)c;
        seq_t res = mod_pow((seq_t)n, (uint64_t)(c - 1), modulus);

        if (res != (seq_t)1)
            return (seq_t)c;
    }
}
