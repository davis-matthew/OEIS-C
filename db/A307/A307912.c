/*
 * A307912  a(n) = n - 1 - pi(2*n-1) + pi(n), where pi is the prime counting
 *          function.
 * https://oeis.org/A307912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 - pi(2*n-1) + pi(n)
 * Range:     u8 n <= 305, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 9474 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence starts at n=1, but guard safety */
        return 0;

    /* prime_pi returns seq_t; we cast to idx_t for index arithmetic */
    idx_t limit = 2 * n - 1;
    seq_t pi2 = prime_pi(limit);   /* pi(2*n-1) */
    seq_t pin = prime_pi(n);       /* pi(n) */

    /* Use signed arithmetic to avoid underflow; intermediate fits in int64_t */
    int64_t diff = (int64_t)n - 1
                   - (int64_t)pi2
                   + (int64_t)pin;

    return (seq_t)diff;             /* diff is non‑negative */
}
