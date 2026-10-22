/*
 * A317489  Irregular triangle read by rows. For n >= 3 and 1 <= k <=
 *          floor(n/3), T(n,k) is the number of palindromic compositions of n
 *          into k parts of size at least 3.
 * https://oeis.org/A317489
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Enumerate the irregular triangle T(n,k) row‐by‐row. T(n,k) = 0
 *            if n odd and k even T(n,k) = binomial((n-1)/2 - k, (k-1)/2) if n
 *            odd and k odd T(n,k) = binomial((n-2)/2 - k, (k-1)/2) if n even
 *            and k odd T(n,k) = binomial((n-2)/2 - k, (k-2)/2) if n even and
 *            k even The OEIS sequence a(n) lists all T...
 * Range:     u8 n <= 137, u16 n <= 137, u32 n <= 137, u64 n <= 137,
 *            u128 n <= 137, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(3) is the first term; for n < 3 the value is undefined, return 0 */
    if (n < 3)
        return (seq_t)0;

    /* target position counting from 1 for n=3 */
    idx_t t = n - 2;          /* t ∈ [1, …] */

    /* Find the row R such that the cumulative number of entries up to row R
       is at least t.  Row lengths are floor(R/3). */
    idx_t R = 3;
    idx_t cum = 0;            /* entries before the current row */

    while (1) {
        idx_t len = R / 3;    /* floor(R/3) */
        if (cum + len >= t)
            break;
        cum += len;
        R++;
    }

    /* Column index k (1‑based) within this row */
    idx_t k = t - cum;        /* 1 <= k <= floor(R/3) */

    seq_t res = (seq_t)0;     /* default for forbidden cases */

    /* Determine the appropriate binomial based on parities of R and k */
    if ((R & 1) && !(k & 1)) {                 /* R odd, k even */
        /* forbidden: return 0 */
    } else if ((R & 1) && (k & 1)) {           /* R odd, k odd */
        idx_t a_val = (R - 1) / 2 - k;
        idx_t b_val = (k - 1) / 2;
        res = binomial(a_val, b_val);
    } else if (!(R & 1) && (k & 1)) {         /* R even, k odd */
        idx_t a_val = (R - 2) / 2 - k;
        idx_t b_val = (k - 1) / 2;
        res = binomial(a_val, b_val);
    } else {                                   /* R even, k even */
        idx_t a_val = (R - 2) / 2 - k;
        idx_t b_val = (k - 2) / 2;
        res = binomial(a_val, b_val);
    }

    return res;
}
