/*
 * A275320  Least k such that d(k*n) = d(k*n+1) where d = A000005, or -1 if no
 *          such k exists.
 * https://oeis.org/A275320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that d(k*n) = d(k*n+1), d = A000005 (return
 *            -1 if no such k exists)
 * Range:     u8 n <= 11, u16 n <= 239, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* n is at least 1 according to the specification */
    uint64_t un = (uint64_t)n;

    for (uint64_t k = 1; ; ++k)
    {
        /* stop if multiplication would overflow 64 bits; then we
           can safely return -1 since we can no longer factor the
           products. */
        if (k > UINT64_MAX / un)
            return (seq_t)-1;

        uint64_t mn  = k * un;            /* k*n  */
        uint64_t mn1 = mn + 1;            /* k*n+1 */

        if (divisor_count_u64(mn) == divisor_count_u64(mn1))
            return (seq_t)k;
    }
}
