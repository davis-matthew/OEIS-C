/*
 * A378027  a(n) = prime(prime(prime(n))) - prime(prime(n)).
 * https://oeis.org/A378027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(prime(n))) - prime(prime(n))
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* First prime at index n */
    seq_t p = (seq_t)get_prime((idx_t)n);

    /* Second prime: prime(p) */
    seq_t q = (seq_t)get_prime((idx_t)p);

    /* Third prime: prime(q) */
    seq_t r = (seq_t)get_prime((idx_t)q);

    /* Difference */
    return r - q;
}
