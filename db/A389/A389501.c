/*
 * A389501  Numbers whose coreful divisors have a noninteger arithmetic mean.
 * https://oeis.org/A389501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer k such that the arithmetic *
 *            mean of the coreful divisors of k is non‑integer. * This is
 *            found by scanning integers until the n-th qualifying number * is
 *            reached
 * Range:     u8 n <= 25, u16 n <= 6203, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   coreful_mean_noninteger
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/coreful_mean_noninteger.h"

/* external entry point as required by the build system */
extern seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n = 0, but safe */

    uint64_t target  = (uint64_t)n;
    uint64_t found   = 0;
    uint64_t k       = 2;        /* start scanning from 2 */

    while (found < target) {
        if (coreful_mean_noninteger(k)) {
            ++found;
            if (found == target) return (seq_t)k;
        }
        ++k;
    }

    return (seq_t)0;   /* should never be reached */
}
