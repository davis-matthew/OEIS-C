/*
 * A377521  Antidiagonal sums of A343053.
 * https://oeis.org/A377521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 0, a(1) = 0, a(2) = 0 * a(n) = (2 - n)*(12*(1 + (-1)^n)
 *            - 105*n + 3*(-1)^n*n - 32*n^2 - 4*n^3)/48 * (for n > 2) * This
 *            is equivalent to the simplified piecewise form used below
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle the initial zeros explicitly. */
    if (n <= 2)
        return (seq_t)0;

    /* Work with seq_t throughout to accommodate arbitrary‑precision types. */
    seq_t n_seq = (seq_t)n;          /* n as seq_t */
    seq_t n2 = n_seq * n_seq;        /* n^2   */
    seq_t n3 = n2 * n_seq;           /* n^3   */

    /* Compute the positive term depending on parity of n. */
    seq_t pos_term;
    bool is_even = ((n & 1ULL) == 0); /* true if n is even */

    if (is_even) {
        /* Even n: pos_term = 4*n^3 + 32*n^2 + 102*n - 24 */
        pos_term = (n3 * (seq_t)4)
                 + (n2 * (seq_t)32)
                 + (n_seq * (seq_t)102)
                 - (seq_t)24;
    } else {
        /* Odd n:  pos_term = 4*n^3 + 32*n^2 + 108*n */
        pos_term = (n3 * (seq_t)4)
                 + (n2 * (seq_t)32)
                 + (n_seq * (seq_t)108);
    }

    /* a(n) = (n-2) * pos_term / 48 */
    seq_t prod = (n_seq - (seq_t)2) * pos_term;
    return prod / (seq_t)48;
}
