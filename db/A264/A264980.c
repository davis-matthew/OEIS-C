/*
 * A264980  Base-3 reversal of 2^n: a(n) = A030102(A000079(n)).
 * https://oeis.org/A264980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A030102(2^n) (reversal of base‑3 digits of 2^n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 510 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return a(idx_t n) with the value of the n‑th term of OEIS A264980. */

#include <stdbool.h>
#include "../utils/pow2.h"

/* Helper: reverse the base‑3 representation of x and return the integer value. */
static seq_t reverse_base3(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        seq_t d = x % 3;      /* least significant base‑3 digit */
        rev = rev * 3 + d;    /* prepend this digit on the right side */
        x = x / 3;            /* drop the processed digit */
    }
    return rev;
}

seq_t a(idx_t n) {
    seq_t val = pow2(n);      /* compute 2^n */
    return reverse_base3(val);
}
