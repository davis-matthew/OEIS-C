/*
 * A278528  a(n) = number of the round in which n is removed in the Flavius
 *          sieve, 0 if it is never removed (when n is one of the terms of
 *          A000960).
 * https://oeis.org/A278528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [C>0] * C C is the difference A-B of the last round
 *            before A and B become equal in the Flavius sieve process
 *            described in the OEIS entry
 * Range:     u8 n <= 292, u16 n <= 9314, u32 n <= 9314, u64 n <= 9314,
 *            u128 n <= 9314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset starts at 1 */

    seq_t A = (seq_t)n;
    seq_t B = (seq_t)n - (seq_t)1;
    seq_t last_pos_diff = (seq_t)0;
    bool last_pos = false;
    idx_t m = 0;

    while (A != B) {
        ++m;                                     /* round number */

        /* C = A - B; record if positive */
        if (A > B) {
            last_pos_diff = A - B;
            last_pos = true;
        } else {
            last_pos = false;
        }

        /* compute A_new = |A - m - (A mod m)| */
        seq_t a_mod = A % (seq_t)m;
        seq_t subA  = (seq_t)m + a_mod;
        if (A >= subA) {
            A = A - subA;
        } else {
            A = subA - A;
        }

        /* compute B_new = |B - m - (B mod m)| */
        seq_t b_mod = B % (seq_t)m;
        seq_t subB  = (seq_t)m + b_mod;
        if (B >= subB) {
            B = B - subB;
        } else {
            B = subB - B;
        }
    }

    return last_pos ? last_pos_diff : (seq_t)0;
}
