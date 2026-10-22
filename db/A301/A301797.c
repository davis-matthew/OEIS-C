/*
 * A301797  a(n) = (4^prime(n) - 1)/3.
 * https://oeis.org/A301797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^prime(n)-1)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Fetch the n‑th prime (offset 1). */
    uint32_t p = get_prime(n);

    /* Compute 4^p as a seq_t. */
    seq_t val = seq_pow((seq_t)4, (idx_t)p);

    /* Compute (4^p - 1) / 3. */
    val = (val - (seq_t)1) / (seq_t)3;
    return val;
}
