/*
 * A337241  a(n) is the least k such that the decimal representation of k*n
 *          contains at least two digits which are the same.
 * https://oeis.org/A337241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that n*k has at least one repeated digit
 * Range:     u8 n <= 25, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_duplicate_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search over k, checking digits of the product. */

#include <stdbool.h>
#include "../utils/has_duplicate_digits.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* Convert index to seq_t */
    for (seq_t k = 1;; ++k) {    /* Search from k = 1 upward */
        seq_t prod = N * k;
        if (has_duplicate_digits(prod))
            return k;
    }
}
