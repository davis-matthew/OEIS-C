/*
 * A322796  a(n) = Kronecker symbol (6/n).
 * https://oeis.org/A322796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (6/n)
 * Range:     i8 n <= 9127, i16 n <= 9127, i32 n <= 9127, i64 n <= 9127,
 *            i128 n <= 9127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This symbol is periodic modulo 24:
 * 0, 1, 0, 0, 0, 1, 0, -1, 0, 0, 0, -1,
 * 0, -1, 0, 0, 0, -1, 0, 1, 0, 0, 0, 1.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[24] = {
        (seq_t)0,  (seq_t)1,  (seq_t)0,  (seq_t)0,
        (seq_t)0,  (seq_t)1,  (seq_t)0,  (seq_t)(-1),
        (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)(-1),
        (seq_t)0,  (seq_t)(-1),(seq_t)0,  (seq_t)0,
        (seq_t)0,  (seq_t)(-1),(seq_t)0,  (seq_t)1,
        (seq_t)0,  (seq_t)0,  (seq_t)0,  (seq_t)1
    };
    return tbl[(size_t)(n % 24)];
}
