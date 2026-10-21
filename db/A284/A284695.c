/*
 * A284695  Greatest prime number appearing in the prime tower factorization
 *          of n (when n > 1); a(1) = 1.
 * https://oeis.org/A284695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime appearing in the prime‑tower factorization
 *            of n (n>1); a(1)=1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tower_max_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tower_max_prime.h"

/* OEIS sequence A284695 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* safety: offset starts at 1 */
    return tower_max_prime((uint64_t)n);
}
