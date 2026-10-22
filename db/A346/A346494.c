/*
 * A346494  Heptagonal numbers (A000566) with prime indices (A000040).
 * https://oeis.org/A346494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * (5*prime(n)-3) / 2 (OEIS A346494)
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = get_prime(n);          /* nth prime, 1‑indexed */
    seq_t v = 5 * p - 3;             /* (5p - 3) as seq_t */
    return p * v / 2;                /* compute a(n) */
}
