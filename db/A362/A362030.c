/*
 * A362030  Irregular triangle read by rows where row n contains the balanced
 *          binary words of length 2n interpreted as binary numbers.
 * https://oeis.org/A362030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n-th lexicographically ordered balanced binary word
 *            of even length 2m (with m zeros and m ones), interpreted as a
 *            binary number
 * Range:     u8 n <= 34, u16 n <= 9426, u32 n <= 9647, u64 n <= 9647,
 *            u128 n <= 9647, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Multiplicative computation of binomial coefficient C(n,k) in seq_t. */
static inline seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t m = 1;                     /* row number (length = 2*m) */
    seq_t cumulative = 0;            /* cumulative count up to current row */
    seq_t row_sum = 0;               /* C(2*m,m) */

    for (;;) {
        row_sum = binom_multiplicative(2 * m, m);
        cumulative += row_sum;
        if (cumulative >= (seq_t)n)
            break;
        ++m;
    }

    /* 1‑based position of the target string within its row */
    seq_t pos = (seq_t)n - (cumulative - row_sum);

    seq_t value = 0;                 /* result as binary number */
    idx_t zeros_left = m;            /* zeros that still need to be placed */

    for (idx_t i = 0; i < 2 * m; ++i) {
        idx_t rem = 2 * m - i;       /* remaining positions to fill */
        seq_t cnt0 = 0;              /* number of strings starting with 0 */

        if (zeros_left > 0)
            cnt0 = binom_multiplicative(rem - 1, zeros_left - 1);

        if (pos <= cnt0) {           /* choose 0 */
            --zeros_left;
            value = (value << 1);
        } else {                     /* choose 1 */
            pos -= cnt0;
            value = (value << 1) | 1;
        }
    }

    return value;
}
