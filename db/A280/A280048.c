/*
 * A280048  The Tower of Hanoi word, with a,b,c,a-bar,b-bar,c-bar encoded as
 *          1,2,3,-1,-2,-3 respectively.
 * https://oeis.org/A280048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = encode_move(p_prev, p_next) where disk d = v2(n+1)+1,
 *            direction dir = 1 if d is odd (clockwise), 2 if d is even
 *            (counter‑clockwise), p_prev = ( floor((n+1)/2^d) * dir ) mod 3,
 *            p_next = ( p_prev + dir ) mod 3, and encode_move maps the
 *            directed edge (src→dst) to 1 for 0→1 (a), 2 for 1→2 (b),...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   encode_move
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/encode_move.h"

seq_t a(idx_t n)
{
    /* m = n + 1 (1‑based step number) */
    idx_t m = n + 1;

    /* compute v2(m): trailing zero count of m */
    int tz = 0;
    idx_t tmp = m;
    while ((tmp & 1ULL) == 0ULL)
    {
        tmp >>= 1;
        ++tz;
    }
    /* disk number (1‑based) */
    int disk = tz + 1;

    /* direction: 1 = clockwise if disk odd, 2 = counter‑clockwise if disk even */
    int dir = (disk & 1) ? 1 : 2;

    /* number of completed moves of this disk before step n */
    idx_t x;
    if (disk >= 64)          /* undefined shift, but with small n this cannot happen */
        x = 0;
    else
        x = m >> disk;

    /* compute previous peg modulo 3 safely */
    idx_t x_mod = x % 3;
    idx_t p_prev = (x_mod * dir) % 3;
    idx_t p_next = (p_prev + dir) % 3;

    return encode_move((int)p_prev, (int)p_next);
}
