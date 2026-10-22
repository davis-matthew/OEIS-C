/*
 * A339632  Number of partitions of 2n into two positive integer parts (s,t)
 *          such that s and t have the same number of decimal digits and s*t
 *          is semiprime.
 * https://oeis.org/A339632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} [Omega(k*(2*n-k)) = 2] * [floor(log10(k)) =
 *            floor(log10(2*n-k))]
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   bigomega, digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function uses utility helpers provided by the framework:
 *  - digit_count: returns the number of decimal digits of a uint64_t.
 *  - bigomega  : returns the number of prime factors with multiplicity of an idx_t.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_count.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; values below are undefined */
    seq_t count = 0;
    idx_t two_n = 2 * n;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t other = two_n - k;

        /* Parts must have the same number of decimal digits */
        if (digit_count(k) != digit_count(other)) continue;

        /* Check if the product is a semiprime: Omega(product) == 2 */
        idx_t prod = k * other;            /* product fits in 64‑bits for n ≤ 10 000 */
        if (bigomega(prod) == (seq_t)2) ++count;
    }

    return count;
}
