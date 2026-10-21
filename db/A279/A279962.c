/*
 * A279962  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 221", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A279962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) - 1 for odd n > 2 14 * 2^(n-3) for even n > 2
 *            with a(0)=1, a(1)=1, a(2)=2, a(3)=3. This implements the
 *            closed‑form expressions from the OEIS entry for A279962. The
 *            helper pow2(idx_t) computes 2^idx efficiently for arbitrarily
 *            large seq_t values
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases up to n=3 (the formulas are valid only for n>2). */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    if (n & 1U) {                 /* n is odd */
        /* a(n) = 2^(n-1) - 1 */
        return pow2(n - 1) - (seq_t)1;
    } else {                      /* n is even */
        /* a(n) = 14 * 2^(n-3) */
        return pow2(n - 3) * ((seq_t)14);
    }
}
