/*
 * A249887  Number of length 4+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (630 n^10 + 222 n^9 + 3267 n^8 + 2652 n^7 + 6419 n^6
 *                + 4431 n^5 + 1043 n^4 + 3663 n^3 + 57 n – 334 n^2) / 630
 */

#include <stdint.h>

#ifdef __SIZEOF_INT128__
typedef __int128  i128;
typedef unsigned __int128 u128;
#endif

seq_t a(idx_t n)
{
    /* Coefficients for the positive part of the numerator
     *    630 n^10 + 222 n^9 + 3267 n^8 + 2652 n^7 + 6419 n^6
     *    + 4431 n^5 + 1043 n^4 + 3663 n^3 + 57 n
     */
    static const seq_t pos_coeff[11] = {
        (seq_t)0,      /* n^0 term   */
        (seq_t)57,     /* n^1 term   */
        (seq_t)0,      /* n^2 term   (negative part handled separately) */
        (seq_t)3663,   /* n^3 term   */
        (seq_t)1043,   /* n^4 term   */
        (seq_t)4431,   /* n^5 term   */
        (seq_t)6419,   /* n^6 term   */
        (seq_t)2652,   /* n^7 term   */
        (seq_t)3267,   /* n^8 term   */
        (seq_t)222,    /* n^9 term   */
        (seq_t)630     /* n^10 term  */
    };

    /* Use wide intermediate only when it is available and the fixed‑size
     * result type is small enough.  For arbitrary‑precision types the
     * compiler will discard this branch entirely. */
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit signed arithmetic – safe for all n that fit in the
         * 64‑bit result type. */
        i128 res = (i128)630;
        for (int i = 9; i >= 0; --i) {
            res = res * (i128)n + (i128)pos_coeff[i];
        }
        /* subtract the negative part 334 n² */
        res -= (i128)334 * (i128)n * (i128)n;
        return (seq_t)(res / (i128)630);
    }
#endif

    /* Arbitrary‑precision or 128‑bit unsigned path */
    seq_t res = (seq_t)630;
    for (int i = 9; i >= 0; --i) {
        res = res * n + pos_coeff[i];
    }
    /* subtract 334 n² */
    seq_t n2 = n * n;
    seq_t sub = (seq_t)334 * n2;
    res = res - sub;
    return res / (seq_t)630;
}
