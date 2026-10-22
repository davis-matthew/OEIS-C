/*
 * A351923  Number of ordered pairs of positive integers (s,t), s,t <= n, such
 *          that (s^t) | n.
 * https://oeis.org/A351923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (s,t) with 1 ≤ s,t ≤ n such that
 *            s^t divides n
 * Range:     u8 n <= 15, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t total = 0;
    seq_t n_val = (seq_t)n;

    /* For s = 1, 1^t = 1 divides n for any t ≤ n */
    total += n_val;          /* all t from 1 to n */

    for (idx_t s = 2; s <= n; ++s) {
        seq_t pow_val = (seq_t)s;
        for (; pow_val <= n_val; pow_val *= (seq_t)s) {
            if (n_val % pow_val == 0) {
                ++total;
            }
        }
    }
    return total;
}
