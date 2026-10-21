/*
 * A263054  Number of (n+1) X (2+1) 0..1 arrays with each row and column not
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 3 binary arrays in which – every row,
 *            read as a 3‑bit binary number, is not divisible by 3, – every
 *            column, read as an (n+1)‑bit binary number, is not divisible by
 *            3. Computation is performed by dynamic programming over the
 *            remainders modulo 3 of the column values
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes for the build environment. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mod3.h"

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* Number of rows: (n+1). */
    const idx_t rows = n + (idx_t)1;

    /* Allowed 3‑bit patterns for a row (value not divisible by 3). */
    /* 001 (1), 010 (2), 100 (4), 101 (5), 111 (7) */
    static const int pat[5][3] = {
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 0},
        {1, 0, 1},
        {1, 1, 1}
    };

    /* DP tables: 27 states, one for each (r0,r1,r2) remainder triple. */
    seq_t curr[27] = {0};
    seq_t next[27] = {0};

    /* Initial state: all column remainders are 0. */
    curr[0] = (seq_t)1;

    /* Process each row. */
    for (idx_t r = 0; r < rows; ++r) {
        /* Zero the next table. */
        for (size_t i = 0; i < 27; ++i)
            next[i] = (seq_t)0;

        /* Iterate over all 27 remainder states. */
        for (size_t s = 0; s < 27; ++s) {
            seq_t val = curr[s];
            if (!val) continue;

            /* Decode the state into three remainders. */
            const int rr0 = (int)(s / 9);
            const int rr1 = (int)((s / 3) % 3);
            const int rr2 = (int)(s % 3);

            /* Try each allowed row pattern. */
            for (int k = 0; k < 5; ++k) {
                const int b0 = pat[k][0];
                const int b1 = pat[k][1];
                const int b2 = pat[k][2];

                const int nr0 = mod3(rr0 * 2 + b0);
                const int nr1 = mod3(rr1 * 2 + b1);
                const int nr2 = mod3(rr2 * 2 + b2);

                const size_t ns = (size_t)(nr0 * 9 + nr1 * 3 + nr2);
                next[ns] += val;
            }
        }
        /* Move to next iteration. */
        for (size_t i = 0; i < 27; ++i)
            curr[i] = next[i];
    }

    /* Sum all states where every column remainder is 1 or 2. */
    seq_t result = (seq_t)0;
    for (size_t s = 0; s < 27; ++s) {
        const int rr0 = (int)(s / 9);
        const int rr1 = (int)((s / 3) % 3);
        const int rr2 = (int)(s % 3);
        if (rr0 && rr1 && rr2)
            result += curr[s];
    }
    return result;
}
