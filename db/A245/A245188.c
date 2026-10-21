/*
 * A245188  Trajectory of 1 under repeated applications of the morphism 0->12,
 *          1->13, 2->20, 3->21.
 * https://oeis.org/A245188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol of the fixed point of the morphism 0 →
 *            12, 1 → 13, 2 → 20, 3 → 21, starting from the initial symbol 1
 * Range:     u8 n <= 114, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Left and right children according to the morphism. */
static const seq_t left_child[4]  = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2 };
static const seq_t right_child[4] = { (seq_t)2, (seq_t)3, (seq_t)0, (seq_t)1 };

seq_t a(idx_t n)
{
    if (n == 0)                /* the zeroth term is the initial symbol */
        return (seq_t)1;

    seq_t value = (seq_t)1;    /* start at the root of the tree */
    idx_t mask = (idx_t)1;

    /* Find the highest power of two not exceeding n.  */
    while (mask <= n)
    {
        /* Shift left until the next shift would overflow or exceed n. */
        if (mask > (idx_t)(-1) >> 1)
            break;            /* should never happen for valid 64‑bit indices */
        mask <<= 1;
    }
    while (mask > n)           /* step back to the largest valid power of two */
        mask >>= 1;

    /* Iterate over the bits of n from most significant to least significant. */
    for (; mask; mask >>= 1)
    {
        if (n & mask)                      /* bit = 1 → go right */
            value = right_child[(unsigned)value];
        else                               /* bit = 0 → go left  */
            value = left_child[(unsigned)value];
    }

    return value;
}
