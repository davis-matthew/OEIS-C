/*
 * A270819  a(n) is the number of arithmetic progressions of length 3 among
 *          the quadratic residues modulo prime(n).
 * https://oeis.org/A270819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1) * floor((prime(n)-2)/8)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Index 0 is outside the OEIS offset – return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n‑th prime (1‑indexed). */
    seq_t p = (seq_t)get_prime(n);

    /* Compute the two factors of the formula. */
    seq_t term1 = p - (seq_t)1;
    seq_t term2 = (p >= (seq_t)2) ? ((p - (seq_t)2) / (seq_t)8) : (seq_t)0;

    /* Multiply in a wider type to avoid premature overflow. */
    return (seq_t)((unsigned long long)term1 * (unsigned long long)term2);
}
