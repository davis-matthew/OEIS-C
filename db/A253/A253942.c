/*
 * A253942  a(n) = 3*binomial(n+1, 5).
 * https://oeis.org/A253942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(n+1, 5)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 177, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 999 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    return 3 * binomial(n + 1, 5);
}
