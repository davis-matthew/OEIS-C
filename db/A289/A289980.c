/*
 * A289980  Oblong (or pronic) numbers of the form x_n.y_n where z_n indicates
 *          the digit z repeated n times.
 * https://oeis.org/A289980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 32, u128 n <= 61,
 *            big any n
 * Helpers:   rep_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289980: oblong (pronic) numbers whose decimal representation is of the
 * form x_n y_n where x and y are digits and each block has the same length n.
 * The sequence consists of:
 *   - the products d*(d+1) for d = 3,4,5,6,7,8,9 (two‑digit numbers), and
 *   - for each k ≥ 2 the three infinite subsequences:
 *        rep_digit(3,k)*(rep_digit(3,k)+1),
 *        rep_digit(6,k)*(rep_digit(6,k)+1),
 *        rep_digit(9,k)*(rep_digit(9,k)+1).
 * These terms are listed in increasing order.
 *
 * Implementation notes:
 *   * The first 7 terms are stored in a small lookup table.
 *   * Subsequent terms are generated on the fly using the index; each block of
 *     three terms corresponds to a fixed k (= 2,3,4,…).
 *   * rep_digit(d,k) builds the integer consisting of the digit d repeated k
 *     times, using only seq_t arithmetic.
 */

#include <stdint.h>
#include "../utils/rep_digit.h"

seq_t a(idx_t n)
{
    /* first seven terms explicitly */
    static const seq_t first7[7] = {12,20,30,42,56,72,90};
    if (n <= 7)  return first7[n-1];

    /* index relative to the first term after the first 7 */
    idx_t idx = n - 7;                       /* 1‑based index for the infinite part */
    idx_t k   = 2 + ((idx - 1) / 3);         /* replication length k ≥ 2   */
    int    r   = (int)((idx - 1) % 3);       /* which subsequence: 0→3, 1→6, 2→9 */

    seq_t x = rep_digit(3, k);
    seq_t y = rep_digit(6, k);
    seq_t z = rep_digit(9, k);

    switch (r) {
        case 0: return x * (x + 1);          /* 3_k * (3_k + 1) */
        case 1: return y * (y + 1);          /* 6_k * (6_k + 1) */
        default: return z * (z + 1);          /* 9_k * (9_k + 1) */
    }
}
