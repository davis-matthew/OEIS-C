/*
 * A359194  Binary complement of 3*n.
 * https://oeis.org/A359194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Binary complement of 3*n
    a(n) = A035327(3*n)
    = (2^k - 1) - 3n,  where k = max(1, floor(log2(3n)) + 1)   */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* m = 3 * n  (computed in seq_t space) */
    seq_t m = ((seq_t)n) * 3;

    /* compute the number of bits needed to represent m, at least 1 */
    size_t len = 0;
    seq_t tmp = m;
    while (tmp) {
        tmp >>= 1;
        ++len;
    }
    if (len == 0)
        len = 1;          /* case m == 0 */

    /* compute mask = 2^len - 1, handling the full-width case */
    size_t bits = sizeof(seq_t) * 8;
    seq_t mask;
    if (len == bits) {
        mask = (seq_t)-1;     /* all bits set */
    } else {
        mask = (((seq_t)1) << len) - 1;
    }

    /* binary complement of m */
    return mask - m;
}
