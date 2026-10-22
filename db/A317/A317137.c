/*
 * A317137  a(n) is the number of nonzero triangular numbers <= n-th prime.
 * https://oeis.org/A317137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((sqrt(8*p_n+1)-1)/2) where p_n is the n-th prime
 * Range:     u8 n <= 4, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k); */
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term.  For n == 0 we return 0
       (undefined lower than the offset). */
    if (n == 0)
        return (seq_t)0;

    /* 1. Obtain the n‑th prime (1‑based). */
    seq_t p = (seq_t)get_prime(n);

    /* 2. Compute 8*p + 1.  All arithmetic is done in seq_t. */
    seq_t val = (seq_t)8 * p + (seq_t)1;

    /* 3. Floor square root of the value. */
    seq_t s = floor_sqrt(val);

    /* 4. Result is (s-1)/2. */
    return (s - (seq_t)1) / (seq_t)2;
}
