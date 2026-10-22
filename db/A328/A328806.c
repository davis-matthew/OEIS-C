/*
 * A328806  Row lengths of A276427: largest k such that a partition of n has
 *          k-1 distinct parts i of multiplicity i.
 * https://oeis.org/A328806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A394247(n) + 1 A394247(n) = largest m such that a
 *            partition of n has m distinct parts i of multiplicity i. The
 *            implementation below computes this value by checking the minimal
 *            possible sum of squares of the first m integers and verifying
 *            that the remaining part can be expressed using integ...
 * Range:     u8 n <= 1318, u16 n <= 8987, u32 n <= 8987, u64 n <= 8987,
 *            u128 n <= 8987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t nseq = (seq_t)n;
    idx_t best = 0;

    for (idx_t m = 0; ; ++m) {
        const seq_t mseq = (seq_t)m;
        /* S_m = m(m+1)(2m+1)/6  -- sum of squares of 1..m */
        const seq_t S = mseq * (mseq + 1) * (2 * mseq + 1) / 6;
        if (S > nseq)
            break;

        const seq_t L = nseq - S;
        if (L == 0 || L >= (seq_t)(m + 1))
            best = m;
    }

    return (seq_t)(best + 1);   /* a(n) = A394247(n) + 1 */
}
