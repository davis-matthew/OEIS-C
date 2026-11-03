/*
 * A398496  a(n) is the least k > 0 such that k*n is a factorial base Niven
 *          number.
 * https://oeis.org/A398496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the least k > 0 such that k*n is a factorial base Niven
 *            number
 * Range:     u8 n <= 28, u16 n <= 5812, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 101 terms through n = 9425 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Check if a value is factorial base Niven:
   compute sum of factorial base digits and test divisibility. */
static bool is_factorial_base_niven(seq_t value) {
    seq_t temp = value;
    seq_t sum  = 0;
    idx_t  i   = 1;           /* position counter: divisor = i+1 */
    while (temp > 0) {
        seq_t divisor = (seq_t)(i + 1);
        sum += temp % divisor;
        temp /= divisor;
        ++i;
    }
    /* For positive value, sum cannot be zero. */
    return (value % sum) == 0;
}

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
