/*
 * A367970  Least k such that 5*n*k+1 is a prime.
 * https://oeis.org/A367970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that 5*n*k + 1 is prime
 * Range:     u8 n <= 10, u16 n <= 1518, u32 n <= 2452, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for k by testing 5*n*k+1 for primality. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t so arithmetic uses the target width. */
    seq_t nn = (seq_t)n;
    seq_t k = (seq_t)1;

    while (1) {
        seq_t cand = (seq_t)5 * nn * k + (seq_t)1;
        if (is_prime_big(cand))
            return k;
        k = k + (seq_t)1;
    }
}
