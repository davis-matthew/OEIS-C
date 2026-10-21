/*
 * A268389  a(n) = greatest k such that polynomial (X+1)^k divides the
 *          polynomial (in polynomial ring GF(2)[X]) that is encoded in the
 *          binary expansion of n. (See the comments for details).
 * https://oeis.org/A268389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 617, u16 n <= 9762, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A268389):
   a(n) = the largest k such that (X+1)^k divides the polynomial
   whose binary coefficients are the bits of n, over GF(2)[X].
   This implementation repeatedly divides the polynomial by (X+1)
   while the sum of coefficients is even, counting the number of
   successful divisions. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;   /* polynomial represented by n */
    seq_t cnt = 0;          /* number of successful divisions */

    for (;;) {
        /* Check parity of the coefficient sum (popcount mod 2). */
        seq_t x = cur;
        int parity = 0;
        while (x) {
            parity ^= (int)(x & 1);
            x >>= 1;
        }
        if (parity)    /* odd popcount → not divisible by X+1 */
            break;

        /* Compute the degree (bit length) of cur. */
        idx_t bits = 0;
        seq_t tmp = cur;
        while (tmp) {
            ++bits;
            tmp >>= 1;
        }

        /* Divide cur by (X+1) over GF(2). */
        seq_t next = 0;
        seq_t cum  = 0;
        /* The quotient has degree bits-2, so run i < bits-1. */
        /* If bits is 0 (should not happen for n ≥ 1), the loop is skipped. */
        for (idx_t i = 0; i + 1 < bits; ++i) {
            seq_t bit = (cur >> i) & (seq_t)1;
            cum ^= bit;             /* cumulative XOR = coefficient of X^i in quotient */
            next |= cum << i;       /* set that coefficient */
        }

        cur = next;
        ++cnt;
    }

    return cnt;
}
