/*
 * A272011  Irregular triangle read by rows: strictly decreasing sequences of
 *          nonnegative numbers given in lexicographic order.
 * https://oeis.org/A272011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the exponent (index of a power of two) appearing as the
 *            n-th element when all binary exponent lists of the integers
 *            1,2,3,… are concatenated in row‑by‑row lexicographic order
 * Range:     u8 n <= 9579, u16 n <= 9579, u32 n <= 9579, u64 n <= 9579,
 *            u128 n <= 9579, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)      /* safety; sequence is defined for n ≥ 1 */
        return 0;

    idx_t cum = 0;          /* cumulative total of terms up to previous row */
    idx_t row = 1;          /* current integer whose binary exponents form a row */

    /* Find the row that contains the n‑th term */
    for (;; ++row) {
        unsigned len = __builtin_popcountll(row);   /* number of 1‑bits in row */
        if (cum + len >= n)
            break;
        cum += len;
    }

    /* Zero‑based position of desired term inside this row */
    idx_t pos_in_row = n - cum - 1;

    /* Walk the bits of 'row' from most significant to least to locate the
       pos_in_row‑th exponent.                                    */
    idx_t exponent = 0;
    unsigned seen = 0;
    for (int bit = 63; bit >= 0; --bit) {
        if (row & (1ULL << bit)) {
            if (seen == pos_in_row) {
                exponent = (idx_t)bit;
                break;
            }
            ++seen;
        }
    }

    return (seq_t)exponent;
}
