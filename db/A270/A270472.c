/*
 * A270472  Expansion of g.f. (1-2*x)/(1-9*x).
 * https://oeis.org/A270472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n)=7*9^(n-1) for n>0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 905 (2026-11-01)
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
        return (seq_t)1;
    seq_t pow9 = ipow((seq_t)9, (seq_t)(n - 1));
    return (seq_t)7 * pow9;
}
