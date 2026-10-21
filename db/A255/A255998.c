/*
 * A255998  Number of length n+7 0..1 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 21*n^2 + 908*n + 384)/6 (n>5) Fixed initial terms
 *            a(1)=256, a(2)=512, a(3)=667, a(4)=796, a(5)=945. (The
 *            polynomial produces the correct values for all n≥6.)
 * Range:     u8 none, u16 n <= 28, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly, as the formula does not
       apply to n ≤ 5. */
    if (n <= 5) {
        switch ((idx_t)n) {
            case 1u: return (seq_t)256;
            case 2u: return (seq_t)512;
            case 3u: return (seq_t)667;
            case 4u: return (seq_t)796;
            case 5u: return (seq_t)945;
            /* no default needed – the switch covers all cases */
        }
    }

    /* For n ≥ 6 use the cubic polynomial formula. */
    seq_t nn = (seq_t)n;          /* n as seq_t */
    seq_t n2 = nn * nn;           /* n² */

    seq_t res = nn * n2;          /* n³ */
    res += (seq_t)21 * n2;        /* + 21·n² */
    res += (seq_t)908 * nn;       /* + 908·n */
    res += (seq_t)384;            /* + 384 */

    return res / 6;               /* divide by 6 */
}
