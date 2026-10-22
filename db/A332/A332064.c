/*
 * A332064  a(1) = 1, a(n + 1) = a(n) -(-1)^a(n) Sum_{k = 1..n}
 *          floor(log_2(a(k)) + 1): depending on parity of a(n), add or
 *          subtract the total number of bits of (absolute values of) the
 *          terms so far.
 * https://oeis.org/A332064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1 a(n+1) = a(n) - (-1)^a(n) * S_n where S_n =
 *            \sum_{k=1..n} ( |a(k)|==0 ? 1 : floor(log₂(|a(k)|)) + 1 ) zero
 *            terms are treated as having 1 bit
 * Range:     i8 n <= 12, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but return 0 for safety */

    seq_t a_curr = 1;                /* a(1) */
    seq_t sum_bits = 0;              /* cumulative sum of bits up to current term */

    /* Add bits of a(1) to sum_bits */
    seq_t abs_a = a_curr;
    if (abs_a < 0) abs_a = -abs_a;
    unsigned bits = (abs_a == 0) ? 1 : bitlen_seq_t(abs_a);
    sum_bits += bits;

    if (n == 1) return a_curr;

    for (idx_t i = 1; i < n; ++i) {          /* i represents current index of a_curr (a(i)) */
        /* sign = (-1)^a_curr : -1 if a_curr odd, +1 if even */
        int sign = ( (a_curr & (seq_t)1) ? -1 : 1 );
        seq_t next = a_curr - ((seq_t)sign * sum_bits);
        a_curr = next;

        /* If we reached the desired term, exit */
        if (i + 1 == n) break;

        /* Add bits of the new term to sum_bits for the next iteration */
        abs_a = a_curr;
        if (abs_a < 0) abs_a = -abs_a;
        bits = (abs_a == 0) ? 1 : bitlen_seq_t(abs_a);
        sum_bits += bits;
    }

    return a_curr;
}
