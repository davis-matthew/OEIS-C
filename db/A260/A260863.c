/*
 * A260863  Base-13 representation of a(n) is the concatenation of the base-13
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the base‑13 representations * of
 *            1,2,…,n,n−1,…,2,1. * Variables a_idx_t and a_seq_t are supplied
 *            by the build
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Pre‑compute powers of 13 up to 10 digits – enough for n ≤ 10 000. */
    static seq_t pow13[11];
    static bool pow_init = false;
    if (!pow_init) {
        pow13[0] = 1;
        for (int k = 1; k <= 10; ++k)
            pow13[k] = pow13[k - 1] * 13;
        pow_init = true;
    }

    seq_t res = 0;

    /* Append 1 … n. */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;
        int digits = 1;
        while (x >= 13) {
            x /= 13;
            ++digits;
        }
        res = res * pow13[digits] + (seq_t)i;
    }

    /* Append n−1 … 1. */
    for (idx_t i = n - 1; i >= 1; --i) {
        idx_t x = i;
        int digits = 1;
        while (x >= 13) {
            x /= 13;
            ++digits;
        }
        res = res * pow13[digits] + (seq_t)i;
    }

    return res;
}
