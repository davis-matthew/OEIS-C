/*
 * A350109  a(n) = Sum_{k=1..n} k * floor(n/k)^n.
 * https://oeis.org/A350109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * floor(n/k)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 363 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 0;
    for (idx_t l = 1; l <= n; )
    {
        idx_t q = n / l;          /* floor(n/l) */
        idx_t r = n / q;          /* last k with the same floor value */
        idx_t count = r - l + 1;

        seq_t sum_k = ((seq_t)(l + r)) * (seq_t)count / 2;     /* sum k from l to r */
        seq_t power = ipow((seq_t)q, (seq_t)n);            /* q^n */
        result += power * sum_k;

        l = r + 1;
    }
    return result;
}
