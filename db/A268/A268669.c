/*
 * A268669  a(n) = polynomial quotient (computed over GF(2), result is its
 *          binary encoding) that is left after all instances of polynomial
 *          (X+1) have been factored out of the polynomial that is encoded by
 *          the binary expansion of n. (See comments for details).
 * https://oeis.org/A268669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = result of removing all factors of (X+1) from the
 *            polynomial represented by the binary encoding of n over GF(2)
 * Range:     u8 n <= 255, u16 n <= 9625, u32 n <= 9625, u64 n <= 9625,
 *            u128 n <= 9625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;          /* work on the sequence type */
    for (;;)
    {
        /* Determine whether popcount(m) is odd.  If so m is already the
           desired quotient. */
        seq_t tmp = m;
        unsigned parity = 0;     /* 0 = even, 1 = odd */
        while (tmp)
        {
            parity ^= (unsigned)(tmp & 1);
            tmp >>= 1;
        }
        if (parity)               /* odd popcount → finished */
            return m;

        /* Divide m by (X+1) over GF(2).  The quotient is computed by
           the recurrence: q0 = a0; for i>0  qi = ai XOR q_{i-1} . */
        seq_t q     = 0;
        seq_t prevq = 0;          /* q_{i-1} for the next step */
        idx_t i     = 0;
        seq_t work  = m;
        while (work)
        {
            seq_t cur  = work & 1;                /* current coefficient ai */
            seq_t qbit = (i == 0) ? cur : (cur ^ prevq);
            if (qbit)
                q |= ((seq_t)1 << i);
            prevq = qbit;
            work >>= 1;
            ++i;
        }
        /* The algorithm above generates an extraneous bit at position
           i-1 (the highest bit).  It must be discarded. */
        if (i > 0)
            q &= ~(((seq_t)1 << (i - 1)));

        m = q;                    /* iterate on the new quotient */
    }
}
