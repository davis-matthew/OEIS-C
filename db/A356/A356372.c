/*
 * A356372  a(n) = Sum_{k=1..n} binomial(2*n, k) * A000005(k).
 * https://oeis.org/A356372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n, k) * A000005(k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   binomial, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000005(k) is the number of divisors of k (tau function). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* work within idx_t (unsigned 64‑bit) arithmetic */
    idx_t two_n = 2 * n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binomial(two_n, k);
        seq_t tau = num_divisors((uint64_t)k);
        result += bin * tau;
    }
    return result;
}
