/*
 * A383441  a(n) is the total of iterations needed in the binary GCD algorithm
 *          to compute gcd(n, k) for k = 0..n. The corresponding row of gcds
 *          is row n of A109004.
 * https://oeis.org/A383441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} (# of iterations of the * binary GCD
 *            algorithm (Knuth's Algorithm B) required to compute gcd(n,k))
 * Range:     u8 n <= 70, u16 n <= 7432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bin_gcd_iterations
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bin_gcd_iterations.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* For k = 0 the binary GCD terminates immediately, contributing 0. */
    for (idx_t k = 1; k <= n; ++k) {
        total += bin_gcd_iterations(n, k);
    }
    return total;
}
