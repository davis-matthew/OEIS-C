/*
 * A246029  a(n) = Product_{i in row n of A245562} prime(i).
 * https://oeis.org/A246029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product over runs of 1's in binary expansion of n of
 *            prime(run_length) The sequence S = [1, 2, 3, 5, 7, 11, ...]
 *            where S(0)=1 and S(k)=prime(k) for k≥1. For n = 0 the result is
 *            1. For n > 0, each maximal run of consecutive 1-bits with length
 *            L contributes the L‑th prime
 * Range:     u8 n <= 7646, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t x = n;

    while (x) {
        /* Skip zero bits */
        while ((x & 1) == 0) {
            x >>= 1;
            if (x == 0)
                return prod;
        }

        /* Count a run of consecutive ones */
        idx_t run = 0;
        while ((x & 1) != 0) {
            run++;
            x >>= 1;
        }

        /* Multiply by the corresponding prime */
        prod *= (seq_t)get_prime(run);
    }

    return prod;
}
