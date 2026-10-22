/*
 * A322168  Sequence gives the values of the trace A+D of the 2 X 2 matrices
 *          [[A,B],[C,D]] in a binary tree of the special linear monoid,
 *          SL(2,Z+).
 * https://oeis.org/A322168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4756, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322168 – trace of the 2×2 matrix at node n of the SL(2,ℤ+) binary‑heap tree */

/* Each node is obtained by starting with the identity matrix I and then
   multiplying it on the left by L = [[1,0],[1,1]] for a “left” child
   or by R = [[1,1],[0,1]] for a “right” child.  The node numbers are
   the usual binary‑heap numbering: root = 1, left child = 2i, right
   child = 2i+1.  To compute a(n) we extract the path bits of n after
   the leading 1 (most significant bit) and apply the corresponding
   transformations.  The result is the trace m00+m11 of the matrix.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* identity matrix */
    seq_t m00 = 1, m01 = 0, m10 = 0, m11 = 1;

    /* find the position (index) of the most significant bit of n */
    idx_t tmp = n;
    idx_t msb_pos = 0;              /* floor(log2(n)) */
    while (tmp >> 1) {
        tmp >>= 1;
        msb_pos++;
    }

    /* bits after the leading 1 */
    idx_t bits = n ^ ((idx_t)1 << msb_pos);
    idx_t bits_left = msb_pos;      /* number of bits to process */

    while (bits_left > 0) {
        /* process the next bit from most significant to least */
        idx_t bit = bits & 1;
        if (bit == 1) {
            /* right child: multiply by R = [[1,1],[0,1]] */
            seq_t t00 = m00 + m10;
            seq_t t01 = m01 + m11;
            m00 = t00;
            m01 = t01;
        } else {
            /* left child: multiply by L = [[1,0],[1,1]] */
            seq_t t10 = m00 + m10;
            seq_t t11 = m01 + m11;
            m10 = t10;
            m11 = t11;
        }
        bits >>= 1;
        bits_left--;
    }

    return m00 + m11;
}
