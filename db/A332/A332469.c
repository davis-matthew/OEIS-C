/*
 * A332469  a(n) = Sum_{k=1..n} floor(n/k)^n.
 * https://oeis.org/A332469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 368 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t q = (seq_t)(n / k);
        result += ipow(q, (seq_t)n);
    }
    return result;
}
