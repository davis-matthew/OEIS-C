/*
 * A379405  a(n) = p((n+1)*p(n)), where p(x) = least prime > x; i.e., p =
 *          A151800.
 * https://oeis.org/A379405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p((n+1)*p(n)) where p(x) is the least prime > x
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* smallest prime strictly greater than n */
    uint64_t p_n = next_prime((uint64_t)n);

    /* compute (n+1)*p(n) as seq_t to avoid overflow of intermediate 64‑bit value */
    seq_t prod = ((seq_t)(n + 1)) * (seq_t)p_n;

    /* next_prime works on uint64_t, so cast prod back – it fits for the problem scope */
    uint64_t prod_u64 = (uint64_t)prod;
    uint64_t p_prod = next_prime(prod_u64);

    return (seq_t)p_prod;
}
