/*
 * A377565  a(n) is the least multiple of n with more decimal digits than n.
 * https://oeis.org/A377565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n * ceil(10^digits(n) / n)
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 70 terms through n = 9648 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    int d = num_digits(n_seq);                     /* number of decimal digits of n */
    seq_t pow = pow10((idx_t)d);                  /* 10^d */
    seq_t k = (pow + n_seq - 1) / n_seq;          /* ceil division: ceil(10^d / n) */
    return n_seq * k;
}
