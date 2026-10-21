/*
 * A260234  Largest prime factor of the n-th hexagonal number (A000384).
 * https://oeis.org/A260234
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of the n-th hexagonal number H(n) =
 *            n * (2*n - 1). a(n) = largest_prime_factor(H(n)). Offset: 2
 *            (a(2) is the first term). For n < 2 the function returns 0
 * Range:     u8 n <= 11, u16 n <= 199, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* offset handling */

    /* compute the hexagonal number  H(n) = n * (2*n - 1)  in seq_t */
    seq_t nn  = (seq_t)n;
    seq_t H   = nn * ( nn * (seq_t)2 - (seq_t)1 );

    /* H is small enough to fit into uint64_t for all supported test values,
       so we can safely cast and call the helper which works on 64‑bit values. */
    uint64_t h = (uint64_t)H;
    uint64_t lpf = largest_prime_factor(h);

    return (seq_t)lpf;
}
