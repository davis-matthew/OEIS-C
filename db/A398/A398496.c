/*
 * A398496  a(n) is the least k > 0 such that k*n is a factorial base Niven
 *          number.
 * https://oeis.org/A398496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least k > 0 such that k*n is a factorial base Niven
 *            number
 * Range:     u8 n <= 28, u16 n <= 5812, u32 n <= 9425, u64 n <= 9425,
 *            u128 n <= 9425, big any n
 * Helpers:   is_factorial_base_niven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_factorial_base_niven.h"

/* Public function implementing the sequence. */
seq_t a(idx_t n) {
    /* n >= 1 as per OEIS offset */
    seq_t n_seq = (seq_t)n;
    seq_t k = 1;
    while (true) {
        seq_t prod = n_seq * k;
        if (is_factorial_base_niven(prod))
            return k;
        ++k;
    }
}
