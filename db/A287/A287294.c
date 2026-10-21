/*
 * A287294  Concatenation of sequence (2n-1,2n-3,..,3,1,3,..,2n-3,2n-1).
 * https://oeis.org/A287294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenate the odd numbers 2n-1, 2n-3, …, 3, 1, 3, …,
 *            2n-3, 2n-1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the utility functions `digit_len` and `pow10`
 * to perform the concatenation efficiently.  It iterates over the
 * descending and ascending halves separately, avoiding unsigned
 * underflow in the loops. */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 2n-1 is the largest odd number in the sequence. */
    idx_t m = 2*n - 1;

    seq_t result = 0;

    /* --------------------------------------------
     * Descending part: (2n-1, 2n-3, …, 1)
     * -------------------------------------------- */
    idx_t count_desc = (m + 1) / 2;      /* number of odd terms in descending part */
    for (idx_t i = 0; i < count_desc; ++i) {
        idx_t odd = m - 2*i;            /* odd number in descending order */
        seq_t k = (seq_t)odd;
        seq_t pow10d = pow10((idx_t)digit_len(k));
        result = result * pow10d + k;
    }

    /* --------------------------------------------
     * Ascending part: (3, 5, …, 2n-3, 2n-1)
     * -------------------------------------------- */
    if (m >= 3) {
        idx_t count_asc = (m - 1) / 2;  /* number of odd terms in ascending part */
        for (idx_t i = 0; i < count_asc; ++i) {
            idx_t odd = 3 + 2*i;        /* odd numbers starting from 3 */
            seq_t k = (seq_t)odd;
            seq_t pow10d = pow10((idx_t)digit_len(k));
            result = result * pow10d + k;
        }
    }

    return result;
}
