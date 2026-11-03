/*
 * A353096  a(1) = 4; for n > 1, a(n) = 5*a(n-1) + 5 - n.
 * https://oeis.org/A353096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*5^(n+1) + 4*n - 15)/16
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 5^(n+1). */
    seq_t pow5 = ipow((seq_t)5, (seq_t)(n + 1));

    /* Apply the closed‑form expression. */
    seq_t num = (seq_t)3 * pow5 + (seq_t)4 * (seq_t)n - (seq_t)15;
    return num / (seq_t)16;
}
