/*
 * A265326  n-th prime minus its binary reversal.
 * https://oeis.org/A265326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = prime(n) - binary_reverse(prime(n)) (OEIS A265326)
 * Range:     i8 n <= 31, i16 n <= 3632, i32 n <= 9620, i64 n <= 9620,
 *            i128 n <= 9620, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the n-th prime via get_prime, then reverse its
   binary representation using bit manipulation.  All arithmetic is done
   in seq_t. */

#include <stdbool.h>
#include "../utils/get_prime.h"

static inline seq_t reverse_binary(seq_t v)
{
    seq_t rev = 0;
    /* determine bit length of v */
    unsigned bitlen = 0;
    seq_t tmp = v;
    while (tmp > 0) {
        tmp >>= 1;
        ++bitlen;
    }
    /* reverse bits */
    for (unsigned i = 0; i < bitlen; ++i) {
        rev <<= 1;
        rev |= v & 1;
        v >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* offset is 1; values for n<1 are undefined */

    /* get the n-th prime (1-indexed) */
    uint32_t p32 = get_prime(n);
    seq_t prime = (seq_t)p32;

    /* binary reverse of the prime */
    seq_t rev = reverse_binary(prime);

    /* a(n) = prime - reversed bits */
    return prime - rev;
}
