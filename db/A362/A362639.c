/*
 * A362639  Number of partitions of [n] whose blocks are ordered with
 *          increasing least elements and where block i (except possibly the
 *          last) has size i.
 * https://oeis.org/A362639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 35, u128 n <= 56,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A362639 – Number of partitions of [n] whose blocks are ordered with
    increasing least elements and where block i (except possibly the last)
    has size i.  Computation is via the product:
        a(n) = ∏_{i=1}^{k} C(m_i-1 , size_i-1)
    where k is the unique integer with
        (k-1)k/2 < n ≤ k(k+1)/2,
    m_i is the number of elements remaining before block i,
    and size_i = i for i<k, size_k = n - (k-1)k/2.  */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*  Handle the empty partition */
    if (n == 0)
        return 1;

    idx_t m = n;          /* elements that have not yet been
                              placed in a block */
    seq_t prod = 1;       /* accumulated product of the binomials */

    for (idx_t i = 1; ; ++i) {
        /* sum of block sizes 1 … (i-1) */
        idx_t sum_prev = (i - 1) * i / 2;
        idx_t size_i;

        /* Decide whether block i uses its prescribed size i or
           becomes the last block with the remaining elements.   */
        if (n - sum_prev >= i)
            size_i = i;
        else
            size_i = n - sum_prev;

        /* choose (size_i – 1) additional elements for the current block
           from the m – 1 remaining numbers after picking the least
           element of this block.                               */
        idx_t choose = size_i - 1;
        prod *= binomial(m - 1, choose);

        /* Remove this block's elements and stop if everything is used. */
        m -= size_i;
        if (m == 0)
            break;
    }

    return prod;
}
