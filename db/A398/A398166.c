/*
 * A398166  Numbers k such that all the exponents in the prime factorizations
 *          of k and k+1 are squares.
 * https://oeis.org/A398166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exponents_are_squares, grow_seq, is_square_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the sequence A398166:
 * numbers k such that in the prime factorisations of k and k+1
 * all exponents are perfect squares. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq.h"
#include "../utils/exponents_are_squares.h"
#include "../utils/is_square_uint.h"

/* External function expected by the framework. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t last_checked = 0;

    while (seq_len < n) {
        ++last_checked;
        if (exponents_are_squares((seq_t)last_checked) &&
            exponents_are_squares((seq_t)(last_checked + 1))) {
            grow_seq(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}
