/*
 * A360505  Concatenate the ternary strings for n, n-1, n-2, ..., 2, 1.
 * https://oeis.org/A360505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the base‑3 representations of n, n−1, …, 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    int digits[12];

    for (idx_t k = n; k > 0; --k) {
        idx_t t = k;
        int len = 0;

        /* obtain ternary digits in reverse order */
        while (t > 0) {
            digits[len++] = t % 3;
            t /= 3;
        }

        /* convert digits to a decimal integer (most significant first) */
        seq_t part = 0;
        for (int i = len - 1; i >= 0; --i) {
            part = part * 10 + digits[i];
        }

        /* append to the running result */
        seq_t pow = pow10((idx_t)len);
        result = result * pow + part;
    }

    return result;
}
