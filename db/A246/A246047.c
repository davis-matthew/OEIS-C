/*
 * A246047  Irregular triangle read by rows, of partial serial probabilities
 *          T(n,k)_{2,3} (see "comments" for definitions and explanation).
 * https://oeis.org/A246047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth entry of the irregular triangle of partial
 *            serial probabilities T(n,k) for J={2,3}. The triangle is read
 *            row by row. T(n,k) = binomial((n-(n mod 2)-2k)/2 , 2k+(n mod 2))
 *            * 2^((n+(n mod 2)+2k)/2)
 * Range:     u8 n <= 13, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Number of entries in row n of the triangle */
static inline idx_t _row_len(idx_t n)
{
    idx_t m = n & 1u;                         /* n mod 2 */

    /* For n = 1 the inequality 6k + 3m ≤ n gives no k ≥ 0, but the row
       contains a single zero entry.  Handle this as a special case. */
    if (n < 3*m)        /* only n = 1 satisfies this */
        return 1u;

    idx_t kmax = (n - 3*m) / 6;               /* floor((n-3m)/6) */
    return kmax + 1u;                         /* k = 0 … kmax */
}

/* Compute a single triangle entry T(n,k) */
static inline seq_t _T_value(idx_t n, idx_t k)
{
    idx_t m   = n & 1u;                     /* n mod 2 */
    idx_t a   = (n - m - 2*k) / 2;          /* upper index of binomial */
    idx_t b   = 2*k + m;                    /* lower index of binomial */
    idx_t exp = (n + m + 2*k) / 2;          /* exponent for power of 2 */

    seq_t bin = binomial(a, b);
    seq_t p2  = pow2(exp);

    return bin * p2;
}

/* Public function requested by the build system.  Computes the nth term
   of OEIS sequence A246047. */
seq_t a(idx_t n)
{
    idx_t pos = 0u;           /* number of elements already accounted for */

    for (idx_t r = 0u; ; ++r) {
        idx_t len = _row_len(r);

        if (pos + len > n) {
            idx_t k = n - pos;
            return _T_value(r, k);
        }
        pos += len;
    }
}
