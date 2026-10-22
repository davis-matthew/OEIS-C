/*
 * A375298  Length of the longest winning path in n X n Hex.
 * https://oeis.org/A375298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * For n >= 10: * if n ≡ 3 (mod 8) a(n) = (2*n² - n - 3) / 4 *
 *            else a(n) = (2*n² - n + 1) / 4 * For 1 ≤ n < 10 the terms are
 *            given in the OEIS table
 * Range:     u8 n <= 11, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t pre[9] = {
    /* a(1) .. a(9) */
    (seq_t)1,
    (seq_t)2,
    (seq_t)5,
    (seq_t)8,
    (seq_t)11,
    (seq_t)16,
    (seq_t)23,
    (seq_t)30,
    (seq_t)37
};

seq_t a(idx_t n)
{
    if (n < 10) {
        /* n is guaranteed to be >= 1 by the caller */
        return pre[n - 1];
    }

    /* Compute using integer arithmetic – all operations are truncating. */
    seq_t sq = (seq_t)n * (seq_t)n;          /* n² */
    seq_t two_sq = sq + sq;                  /* 2 * n² */

    if ((n % 8) == 3) {
        /* a(n) = (2*n² - n - 3) / 4 */
        seq_t num = two_sq - (seq_t)n - (seq_t)3;
        return num / (seq_t)4;
    } else {
        /* a(n) = (2*n² - n + 1) / 4 */
        seq_t num = two_sq - (seq_t)n + (seq_t)1;
        return num / (seq_t)4;
    }
}
