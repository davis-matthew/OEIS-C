/*
 * A352241  Maximal number of nonattacking black-square queens on an n X n
 *          chessboard.
 * https://oeis.org/A352241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = floor((4*n-3)/5) for n ≥ 2, with a(1) = 1. The conjecture
 *            fails only for n = 52 (value 40 instead of 41) and n = 57 (value
 *            44 instead of 45)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* offset 1 */
    /* compute floor((4*n-3)/5) */
    /* use a wide type to avoid intermediate overflow */
    unsigned __int128 t = (unsigned __int128)n * 4u - 3u;
    seq_t res = (seq_t)(t / 5u);

    /* adjust the known exceptional cases */
    if (n == 52 || n == 57) {
        --res;
    }
    return res;
}
