/*
 * A269304  a(n) = n + n/gpf(n) + 1, where gpf(n) is the greatest prime factor
 *          of n or 1 if n = 1.
 * https://oeis.org/A269304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   a(n) = n + n/gpf(n) + 1
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9617 (2026-11-01)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;          /* convert index to plain integer */
    uint64_t g   = largest_prime_factor(n64);             /* greatest prime factor, 1 if n = 1 */
    seq_t n_seq  = (seq_t)n64;           /* promote to the output type */

    /* compute n + n/gpf(n) + 1, all in seq_t arithmetic */
    return n_seq + n_seq / (seq_t)g + (seq_t)1;
}
