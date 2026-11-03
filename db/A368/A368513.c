/*
 * A368513  a(n) = (6*n+1)!/(n!*(2*n)!*(3*n+1)!).
 * https://oeis.org/A368513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n+1)!/(n!*(2*n)!*(3*n+1)!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent combinatorial expression: a(n) = C(6*n+1, n) * C(5*n+1, 2*n) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;                    /* handles the offset 0 */

    /* compute C(6*n+1, n) */
    idx_t six_n_plus1 = (idx_t)6 * n + 1;
    seq_t t1 = binomial(six_n_plus1, n);

    /* compute C(5*n+1, 2*n) */
    idx_t five_n_plus1 = (idx_t)5 * n + 1;
    seq_t t2 = binomial(five_n_plus1, 2 * n);

    /* multiply the two binomial coefficients */
    return seq_mul(t1, t2);
}
