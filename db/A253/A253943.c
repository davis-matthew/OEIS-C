/*
 * A253943  a(n) = 3*binomial(n+1,6).
 * https://oeis.org/A253943
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(n+1,6)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 102, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 901 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    seq_t v = binomial(n + 1, 6);
    return v * 3;
}
