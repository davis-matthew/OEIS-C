/*
 * A270054  Number of nX4 0..4 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling four exactly once.
 * https://oeis.org/A270054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 306*a(n-1) - 27803*a(n-2) + 685150*a(n-3) -
 *            6795613*a(n-4) + 28270996*a(n-5) - 41396356*a(n-6) for n > 7
 *            Initial values: a(1) = 240 a(2) = 99840 a(3) = 22911300 a(4) =
 *            4410058320 a(5) = 779206497960 a(6) = 130850370606120 a(7) =
 *            21244602958557060
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 7 initial terms, stored in seq_t so that they are truncated to
       the width of seq_t (e.g. uint8_t will wrap those that overflow) */
    const seq_t init[7] = {
        (seq_t)240ULL,
        (seq_t)99840ULL,
        (seq_t)22911300ULL,
        (seq_t)4410058320ULL,
        (seq_t)779206497960ULL,
        (seq_t)130850370606120ULL,
        (seq_t)21244602958557060ULL
    };

    if (n <= 7)
        return init[(size_t)n - 1U];

    /* sliding window for the last 7 values */
    seq_t prev[7];
    for (size_t i = 0; i < 7; ++i)
        prev[i] = init[i];

    seq_t cur = 0;
    for (idx_t i = 7; i < n; ++i) {
        cur = (seq_t)306      * prev[6]  /* a(n-1) */
            - (seq_t)27803   * prev[5]  /* a(n-2) */
            + (seq_t)685150  * prev[4]  /* a(n-3) */
            - (seq_t)6795613 * prev[3]  /* a(n-4) */
            + (seq_t)28270996* prev[2]  /* a(n-5) */
            - (seq_t)41396356* prev[1]; /* a(n-6) */

        /* shift the window forward */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = cur;
    }
    return cur;
}
