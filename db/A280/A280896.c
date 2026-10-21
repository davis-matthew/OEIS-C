/*
 * A280896  Number of n X 2 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 24*a(n-1) - 204*a(n-2) + 704*a(n-3)
 *             - 816*a(n-4) + 384*a(n-5) - 64*a(n-6)   for n ≥ 9
 * with initial values
 * a(1)=0, a(2)=1, a(3)=9, a(4)=124, a(5)=1464,
 * a(6)=15768, a(7)=159920, a(8)=1554304
 */
#include <stdlib.h>

static const seq_t base[9] = {
    0,      /* dummy for index 0 */
    0,      /* a(1) */
    1,      /* a(2) */
    9,      /* a(3) */
    124,    /* a(4) */
    1464,   /* a(5) */
    15768,  /* a(6) */
    159920, /* a(7) */
    1554304 /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of bounds – defined as 0 */

    if (n <= 8) return base[n];

    /* previous six terms: a(n-6), a(n-5), …, a(n-1) */
    seq_t prev[6] = {
        base[3], /* a(3)   when n=9 */
        base[4], /* a(4)   */
        base[5], /* a(5)   */
        base[6], /* a(6)   */
        base[7], /* a(7)   */
        base[8]  /* a(8)   */
    };

    for (idx_t k = 9; k <= n; ++k) {
        seq_t next = ((seq_t)24  * prev[5]) -
                     ((seq_t)204 * prev[4]) +
                     ((seq_t)704 * prev[3]) -
                     ((seq_t)816 * prev[2]) +
                     ((seq_t)384 * prev[1]) -
                     ((seq_t)64  * prev[0]);

        /* slide window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = next;
    }

    return prev[5];
}
