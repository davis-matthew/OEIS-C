/*
 * A261138  The concatenation of 123456...n and the reverse of this number.
 * https://oeis.org/A261138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate(1, 2, …, n, rev(n), rev(n‑1), …, rev(1)),
 *            where rev(x) is the decimal reverse of x preserving the number
 *            of digits of x (e.g. rev(10) = 01). The implementation builds
 *            the number iteratively by multiplying by the appropriate power
 *            of ten and adding the next part
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_idx, num_digits_idx, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/num_digits_idx.h"
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* Concatenate 1,2,…,n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pow10d = ipow_idx(10, num_digits_idx(i));
        res = res * pow10d + (seq_t)i;
    }

    /* Concatenate rev(n), rev(n‑1), …, rev(1) */
    for (idx_t i = n; i > 0; --i) {
        seq_t pow10d = ipow_idx(10, num_digits_idx(i));
        seq_t r = reverse_digits_idx(i);
        res = res * pow10d + r;
    }

    return res;
}
