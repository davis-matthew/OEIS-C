/*
 * A383036  The determinant of the matrix representing a totally
 *          anti-symmetric quasigroup of order 2*n+1.
 * https://oeis.org/A383036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+1)^(2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 96 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* convert the index to the result type */
    seq_t N   = (seq_t)n;
    seq_t base = (seq_t)(2 * n + 1);
    seq_t exp  = (seq_t)(2 * n);

    /* compute (2*n+1)^(2*n) */
    seq_t power = ipow(base, exp);

    /* final result */
    return N * power;
}
