/*
 * A264689  Minimum of the Kamae-Xue measure of randomness for binary strings
 *          of length n.
 * https://oeis.org/A264689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor(n/2)^2 + ceil(n/2)^2 + Σ_{l=2}^{n} minSumSquares(n,
 *            l) where for each l the minimal possible sum of squares of the
 *            occurrence counts of all l‑letter factors is let m = n - l + 1
 *            (number of l‑letter factors in a word of length n) let d = 2^l
 *            (number of possible l‑letter patterns) if m...
 * Range:     u8 n <= 14, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            // undefined below offset 1

    /* zeros and ones in the optimal word */
    seq_t zeros = (seq_t)(n / 2);
    seq_t ones  = (seq_t)(n - zeros);
    seq_t result = zeros * zeros + ones * ones;   /* length‑1 contribution */

    size_t bits = sizeof(seq_t) * 8;   /* shift limit for seq_t */

    for (idx_t l = 2; l <= n; ++l) {
        seq_t m = (seq_t)(n - l + 1);  /* number of l‑letter factors */

        seq_t sumSq;
        if (l < bits) {
            seq_t d = ((seq_t)1 << (size_t)l);   /* 2^l fits in seq_t */
            if (m <= d) {
                sumSq = m;                      /* all distinct */
            } else {
                seq_t q = m / d;
                seq_t r = m % d;
                sumSq = (d - r) * q * q + r * (q + 1) * (q + 1);
            }
        } else {
            /* 2^l would overflow seq_t, so m < d and all factors distinct */
            sumSq = m;
        }
        result += sumSq;
    }
    return result;
}
