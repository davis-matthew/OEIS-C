/*
 * A260858  Base-8 representation of a(n) is the concatenation of the base-8
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the base‑8 concatenation of 1,2,…,n,n‑1,…,1, returned as
 *            a decimal integer
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   oct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/oct_digits.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t res = (seq_t)0;

    /* forward part: 1 … n */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t d = oct_digits(k);
        seq_t tmp = res;
        for (idx_t i = 0; i < d; ++i) tmp *= 8;
        tmp += (seq_t)k;
        res = tmp;
    }

    /* backward part: n‑1 … 1 */
    if (n > 0) {
        for (idx_t k = n-1; k > 0; --k) {
            idx_t d = oct_digits(k);
            seq_t tmp = res;
            for (idx_t i = 0; i < d; ++i) tmp *= 8;
            tmp += (seq_t)k;
            res = tmp;
        }
    }

    return res;
}
