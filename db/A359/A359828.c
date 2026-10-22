/*
 * A359828  Characteristic function for primitive elements of A235992.
 * https://oeis.org/A359828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9000, u16 n <= 9000, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   derivative_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if the arithmetic derivative of n is even, and there is
   no proper divisor d|n (1 < d < n) such that both d and n/d have even
   derivative.  Otherwise a(n) = 0.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/derivative_even.h"

/* Integer square‑root (floor) for unsigned 64‑bit indices. */
static inline idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1)) r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* OEIS offset starts at 1 */

    if (!derivative_even(n)) return 0;  /* first condition fails */

    if (n == 1) return 1;              /* 1 has no proper divisors */

    idx_t limit = isqrt(n);
    for (idx_t d = 2; d <= limit; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            if (derivative_even(d) && derivative_even(e))
                return 0;          /* found a forbidden pair */
        }
    }
    return 1;                         /* primitive under the rule */
}
