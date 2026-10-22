/*
 * A368201  Least k such that 9*n*k+1 is a prime.
 * https://oeis.org/A368201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k ≥ 1 such that 9 · n · k + 1 is prime
 * Range:     u8 n <= 12, u16 n <= 3339, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* If n < 1, return 0 – the offset starts at 1, but this case is safe. */
    if (n == 0)
        return (seq_t)0;

    idx_t k = 1;
    for (;; ++k) {
        /* Compute 9*n*k + 1 using seq_t to avoid overflow of idx_t. */
        seq_t val = (seq_t)9 * (seq_t)n * (seq_t)k + (seq_t)1;
        if (isPrimeGeneric(val))
            return (seq_t)k;
    }
}
