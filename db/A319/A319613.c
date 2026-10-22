/*
 * A319613  a(n) = prime(n) * prime(2n).
 * https://oeis.org/A319613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * prime(2n)
 * Range:     u8 n <= 4, u16 n <= 38, u32 n <= 4636, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1.  For n=0 anybody might call it, we return 0. */
    if (n == 0) return 0;

    /* Retrieve the n-th prime and the 2n-th prime.  get_prime() is 1‑based. */
    seq_t p1 = (seq_t) get_prime(n);
    seq_t p2 = (seq_t) get_prime(2 * n);

    /* Return the product. */
    return p1 * p2;
}
