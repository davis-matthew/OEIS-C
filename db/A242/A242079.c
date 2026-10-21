/*
 * A242079  a(1)=1. a(n)=smallest integer greater than 2*a(n-1) such that the
 *          Nim sequence of the first n terms is ternary, periodic and not
 *          equal to the Nim sequence of the first n-1 terms.
 * https://oeis.org/A242079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1; a(2)=4; a(3)=12; a(4)=28; a(5)=73; a(6)=163; a(7)=343;
   a(n) = 524*2^(n-7) - 181  for n >= 7 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* first seven terms */
    static const seq_t tbl[7] =
    {
        (seq_t)1, (seq_t)4, (seq_t)12,
        (seq_t)28, (seq_t)73, (seq_t)163,
        (seq_t)343
    };

    if (n <= 7) return tbl[n - 1];

    idx_t k = n - 7;
    seq_t val = 1;
    for (idx_t i = 0; i < k; ++i) {
        val <<= 1;                /* val = val * 2 */
    }
    val = val * (seq_t)524;      /* multiply by 524 */
    val -= (seq_t)181;           /* subtract 181 */
    return val;
}
