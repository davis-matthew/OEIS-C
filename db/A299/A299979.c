/*
 * A299979  Lexicographic first sequence of positive integers such that a(n) +
 *          a(n+1) has a digit 9, and no term occurs twice.
 * https://oeis.org/A299979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   lexicographic first permutation of the positive integers such
 *            that a(n) + a(n+1) contains the digit 9, with no term occurring
 *            twice. The function builds the sequence lazily and memoises
 *            earlier values
 * Range:     u8 n <= 250, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   sum_has9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/sum_has9.h"

#define MAXVAL 300000   /* buffer large enough for n up to ~10000 */

/* Main sequence function */
seq_t a(idx_t n)
{
    static uint64_t seq[MAXVAL];          /* store sequence values as 64‑bit */
    static bool used[MAXVAL + 1] = {0};   /* whether a number has been used */
    static idx_t len = 0;                 /* number of terms computed so far */

    if (len == 0) {           /* initialise first term */
        seq[0] = 1;
        used[1] = true;
        len = 1;
        if (n == 1) return (seq_t)seq[0];
    }

    while (len < n) {
        uint64_t prev = seq[len - 1];
        idx_t y = 1;
        for (;;) {
            if (y > MAXVAL) {
                /* buffer overflow – should never occur for the required n */
                return (seq_t)0;
            }
            if (!used[y] && sum_has9(prev, y)) break;
            ++y;
        }
        seq[len] = y;
        used[y] = true;
        ++len;
    }
    return (seq_t)seq[n - 1];
}
