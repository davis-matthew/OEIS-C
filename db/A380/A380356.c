/*
 * A380356  a(0) = 0, a(1) = 1, and for any n > 0, if a(n) is at square
 *          distance from the least integer (in absolute value) not yet in the
 *          sequence and of opposite sign to a(n), say u, then a(n+1) = u,
 *          otherwise a(n+1) = a(n) + sign(a(n)).
 * https://oeis.org/A380356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1; for n>1, let u be the least unused integer of
 *            opposite sign. If |a(n)-u| is a perfect square, a(n+1)=u;
 *            otherwise a(n+1)=a(n)+sign(a(n))
 * Range:     i8 n <= 24, i16 n <= 1302, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t cur = 1;          /* a(1) */
    seq_t next_pos = 2;     /* next unused positive integer */
    seq_t next_neg = -1;    /* next unused negative integer */

    for (idx_t i = 1; i < n; ++i) {
        seq_t u = (cur > 0) ? next_neg : next_pos;
        seq_t diff = (cur > u) ? (cur - u) : (u - cur);

        seq_t s = isqrt_seq(diff);
        if (s * s == diff) {
            cur = u;
        } else {
            cur = cur + (cur > 0 ? 1 : -1);
        }

        if (cur > 0) {
            if (cur == next_pos) ++next_pos;
        } else if (cur < 0) {
            if (cur == next_neg) --next_neg;
        }
    }

    return cur;
}
