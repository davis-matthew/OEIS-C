/*
 * A387325  Self-inverse permutation of the nonnegative integers, by swapping
 *          and converting the head and tail parts of n in binary and using
 *          the rightmost 0-bit as their separator.
 * https://oeis.org/A387325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p << (h+1)) | ((1 << h) - 1) where p = number of
 *            trailing 1-bits in n, and h = n >> (p+1) (the head)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Count the trailing 1-bits */
    idx_t tmp = n;
    idx_t p = 0;
    while ((tmp & (idx_t)1) == (idx_t)1) {
        ++p;
        tmp >>= 1;
    }

    /* Head value (bits above the rightmost 0) */
    seq_t h = (seq_t)(n >> (p + 1));

    /* Shift amounts for the two parts.
       shift_head = h + 1  (to make room for the separator zero),
       shift_tail = h     (length of the block of ones). */
    size_t shift_head = (size_t)h + 1;
    size_t shift_tail = (size_t)h;

    /* The block of ones coming from the old head value. */
    seq_t tail_part = ((seq_t)1 << shift_tail) - ((seq_t)1 << 0);

    /* Result is the new head part (p) shifted left by shift_head,
       OR'ed with the block of ones. */
    seq_t result = ((seq_t)p << shift_head) | tail_part;

    return result;
}
