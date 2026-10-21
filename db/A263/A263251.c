/*
 * A263251  Odd bisection of A263087; number of solutions to x - d(x) =
 *          (2n+1)^2, where d(x) is the number of divisors of x (A000005).
 * https://oeis.org/A263251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x - d(x) = (2n+1)², where d(x) is
 *            the divisor count (A000005)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"

static const idx_t MAX_D = (idx_t)5000;

/* Compute a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* y = (2n+1)²  (computed in unsigned 64‑bit to avoid overflow). */
    uint64_t two_n_plus_one = 2ULL * (uint64_t)n + 1ULL;
    uint64_t y = two_n_plus_one * two_n_plus_one;

    seq_t res = 0;
    for (idx_t d = 1; d <= MAX_D; ++d)
    {
        uint64_t x  = y + (uint64_t)d;          /* x = y + d  */
        uint64_t dc = divisor_count_u64(x);     /* d(x)       */
        if (dc == (uint64_t)d)                  /* x-d = y    */
            res += (seq_t)1;
    }
    return res;
}
