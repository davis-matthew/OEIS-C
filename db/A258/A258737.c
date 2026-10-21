/*
 * A258737  Number of length n+7 0..3 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A258737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 5: a(n) = (n^7 + 77·n^6 + 2527·n^5 + 44975·n^4 +
 *            4572064·n^3 + 31399508·n^2 + 69185808·n + 39553920) / 5040 For n
 *            = 1..5: the values are provided in the table below
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


static const seq_t SMALL[5] = {
    (seq_t)65536,   /* a(1) */
    (seq_t)163020,  /* a(2) */
    (seq_t)220854,  /* a(3) */
    (seq_t)281136,  /* a(4) */
    (seq_t)370510   /* a(5) */
};

seq_t a(idx_t n)
{
    if (n >= 1 && n <= 5)
        return SMALL[n - 1];

    const seq_t nn = (seq_t)n;

    /* Compute powers of n up to n^7.  All calculations stay in seq_t,
       so no overflow occurs for values that fit in seq_t. */
    seq_t p0 = (seq_t)1;
    seq_t p1 = nn;
    seq_t p2 = p1 * nn;
    seq_t p3 = p2 * nn;
    seq_t p4 = p3 * nn;
    seq_t p5 = p4 * nn;
    seq_t p6 = p5 * nn;
    seq_t p7 = p6 * nn;

    /* Build numerator of the rational expression. */
    seq_t num = p7;
    num += (seq_t)77       * p6;
    num += (seq_t)2527     * p5;
    num += (seq_t)44975    * p4;
    num += (seq_t)4572064  * p3;
    num += (seq_t)31399508 * p2;
    num += (seq_t)69185808 * p1;
    num += (seq_t)39553920 * p0;

    /* Division is exact. */
    return num / (seq_t)5040;
}
