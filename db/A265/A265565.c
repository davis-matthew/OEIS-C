/*
 * A265565  Smallest base-5 palindrome m >= n, written in base 5.
 * https://oeis.org/A265565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base-5 palindrome m ≥ n, returned as the decimal
 *            value obtained by interpreting m’s base‑5 digits as a decimal
 *            number
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   base5_as_decimal, is_palindrome_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/base5_as_decimal.h"
#include "../utils/is_palindrome_base5.h"

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    while (!is_palindrome_base5(m)) {
        m++;
    }
    return base5_as_decimal(m);
}
