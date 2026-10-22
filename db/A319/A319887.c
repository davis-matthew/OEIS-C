/*
 * A319887  a(n) = 4*3*2*1 - 8*7*6*5 + 12*11*10*9 - 16*15*14*13 + ... - (up to
 *          the n-th term).
 * https://oeis.org/A319887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the cumulative sum of alternating blocks of four
 *            descending integers: block k (k≥0) = (4k+4)(4k+3)(4k+2)(4k+1)
 *            For each block we add its four partial products with alternating
 *            signs: + (4k+4) - (4k+4)(4k+3) + (4k+4) + (4k+4)(4k+3)(4k+2) -
 *            (4k+4)(4k+3) + (4k+4)(4k+3)(4k+2)(4k+1) - (4k+4)(4...
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(1)=4
             a(2)=12
             a(3)=24
             a(4)=24
             a(5)=16
             a(6)=-32
             a(7)=-312
             a(8)=-1656
             ... */

#include <stdint.h>  /* used only if build defines seq_t and idx_t */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t sum = 0;
    idx_t full_blocks = n / 4;       /* number of completely processed blocks   */
    idx_t rem       = n % 4;         /* remaining terms in the next block (0–3) */

    /* sum over full blocks */
    for (idx_t m = 0; m < full_blocks; ++m) {
        seq_t sign = ((m & 1) == 0) ? 1 : -1;
        seq_t b1 = (seq_t)(4 * m + 4);
        seq_t b2 = (seq_t)(4 * m + 3);
        seq_t b3 = (seq_t)(4 * m + 2);
        seq_t b4 = (seq_t)(4 * m + 1);
        seq_t prod = b1 * b2 * b3 * b4;   /* product of all four numbers */
        sum += sign * prod;
    }

    /* partial block (if any) */
    if (rem > 0) {
        idx_t m = full_blocks;
        seq_t sign = ((m & 1) == 0) ? 1 : -1;
        seq_t b1 = (seq_t)(4 * m + 4);
        seq_t b2 = (seq_t)(4 * m + 3);
        seq_t b3 = (seq_t)(4 * m + 2);
        seq_t b4 = (seq_t)(4 * m + 1);

        seq_t p1 = b1;
        seq_t p2 = p1 * b2;
        seq_t p3 = p2 * b3;
        seq_t p4 = p3 * b4;

        if (rem >= 1) sum += sign * p1;
        if (rem >= 2) sum += sign * (p2 - p1);
        if (rem >= 3) sum += sign * (p3 - p2);
    }

    return sum;
}
