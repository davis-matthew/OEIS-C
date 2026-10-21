/*
 * A249901  a(n) = mu(n) + bigomega(n).
 * https://oeis.org/A249901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008683(n) + A001222(n) (bigomega(n) + mobius(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety – the sequence is defined for n ≥ 1 */
    return bigomega(n) + mobius(n);
}
