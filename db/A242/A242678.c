/*
 * A242678  The number of ways that a score of n can be obtained using three
 *          darts on a standard dartboard.
 * https://oeis.org/A242678
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 180, u16 n <= 180, u32 n <= 180, u64 n <= 180,
 *            u128 n <= 180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to obtain score n with three darts on a standard board.
   Count unordered triples (a,b,c) from the set of possible scores
   (single, double, triple, bull) that sum to n.  Unordered means
   a <= b <= c, so each triple is counted only once. */
#include <stddef.h>  /* size_t */

/* Set of distinct scores achievable with one dart on a standard board */
static const idx_t S_values[43] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 24, 25, 26, 27, 28, 30, 32, 33,
    34, 36, 38, 39, 40, 42, 45, 48, 50, 51,
    54, 57, 60
};
static const size_t S_len = sizeof(S_values) / sizeof(S_values[0]);

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;

    for (size_t i = 0; i < S_len; ++i) {
        idx_t ai = S_values[i];
        if (ai > n) break;          /* further sums will be larger */

        for (size_t j = i; j < S_len; ++j) {
            idx_t bj = S_values[j];
            idx_t sum_ab = ai + bj;
            if (sum_ab > n) break;

            for (size_t k = j; k < S_len; ++k) {
                idx_t ck = S_values[k];
                idx_t sum = sum_ab + ck;
                if (sum == n)
                    ++count;
                if (sum > n)
                    break;
            }
        }
    }

    return count;
}
