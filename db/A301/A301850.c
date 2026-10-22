/*
 * A301850  The Dakota sequence: a sequence with zero-free number-wall over
 *          ternary extension fields.
 * https://oeis.org/A301850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9999, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The Dakota sequence (A301850)
 *
 *  Construction:
 *    Let c(0),c(1),... be the fixed point of the morphism
 *       1 -> 1 3
 *       2 -> 2 3
 *       3 -> 1 4
 *       4 -> 2 4
 *    started with the single letter 1.
 *
 *    The sequence a(n) is obtained by encoding each letter of c as two
 *    values:
 *       1 -> 0 , +1
 *       2 -> +1, -1
 *       3 -> 0 , -1
 *       4 -> +1, 0
 *
 *    Thus a(2k)   = encoding[c(k)].first
 *          a(2k+1) = encoding[c(k)].second
 *
 *  Implementation:
 *    For a given index n, compute k = n/2 (pair index) and p = n%2
 *    (p=0 for the first value, p=1 for the second).
 *    The letter c(k) is derived by starting from 1 and following the
 *    binary representation of k from the most significant bit to the
 *    least.  A bit 0 means “take the first child”, a bit 1 means
 *    “take the second child” according to the tables below.
 *
 *    The algorithm is O(log n) and uses only integer arithmetic.
 */

#include <stdint.h>

/*  Transition tables for the 2‑letter morphism */
static const unsigned char first_child[5]   = {0, 1, 2, 1, 2};
static const unsigned char second_child[5]  = {0, 3, 3, 4, 4};

/*  Table of encoded values for each letter:
 *       letter 1 -> (0 , +1)
 *       letter 2 -> (+1, -1)
 *       letter 3 -> (0 , -1)
 *       letter 4 -> (+1,  0)
 */
static const signed char enc[5][2] = {
    {0, 0},          /* unused 0 index */
    { 0,  1},        /* 1 -> 0 , +1 */
    { 1, -1},        /* 2 -> +1, -1 */
    { 0, -1},        /* 3 -> 0 , -1 */
    { 1,  0}         /* 4 -> +1, 0  */
};

seq_t a(idx_t n)
{
    /* pair index and position inside the pair   */
    idx_t pair   = n >> 1;          /* floor(n/2) */
    unsigned char pos   = (unsigned char)(n & 1);   /* 0 or 1 */

    /* letter at position pair in the infinite word c */
    unsigned char letter = 1;      /* start with the seed 1 */

    if (pair != 0) {
        /* locate the most significant set bit of pair */
        int msb = 63;
        while (!(pair & ((idx_t)1 << msb)))  msb--;
        /* walk from the most significant bit down to 0 */
        for (int i = msb; i >= 0; --i) {
            unsigned char bit = (pair >> i) & 1;
            if (bit == 0)
                letter = first_child[letter];
            else
                letter = second_child[letter];
        }
    }

    /* return the appropriate encoded value */
    return (seq_t)enc[letter][pos];
}
