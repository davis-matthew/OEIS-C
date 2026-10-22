/*
 * A400742  Number of runs of at least two odd steps before the first drop of
 *          4n+3 under T(x) = x/2 (x even), (3x+1)/2 (x odd).
 * https://oeis.org/A400742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = number of runs of at least two consecutive odd steps *
 *            before the first drop of the start value 4*n+3 under the map *
 *            T(x) = x/2 (x even), (3*x+1)/2 (x odd)
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* starting value */
    seq_t start = ((seq_t)4 * n) + 3;
    seq_t x      = start;

    /* number of consecutive odd steps seen since last even */
    unsigned odd_len = 1;   /* the start is odd */
    seq_t   count   = 0;    /* number of qualifying runs */

    for (;;) {
        /* next term of the orbit */
        if ((x & 1) == 0)        /* even */
            x = x >> 1;
        else                     /* odd */
            x = (x * 3 + 1) >> 1;   /* (3*x+1)/2 */

        /* first drop below the starting value? */
        if (x < start)
            break;

        if ((x & 1) == 0) {           /* even reached */
            if (odd_len >= 2)
                count++;
            odd_len = 0;
        } else {                      /* odd reached */
            odd_len++;
        }
    }

    /* count the final run if it ended on a drop and had length >=2 */
    if (odd_len >= 2)
        count++;

    return count;
}
