/*
 * A397643  Numbers with digits in nondecreasing order having only digits 1
 *          through 3.
 * https://oeis.org/A397643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 285, u64 n <= 1749,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397643: the nth nondecreasing integer whose decimal digits are
   1, 2 or 3.  The implementation constructs the kth length block and
   then builds the least‑*n* number within that block using combinatorial
   counting. */

#include <stdint.h>

/* small binomial coefficient for n ≤ 100 and k ≤ 3 */
static idx_t binom_small(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Find the digit length k of the nth number */
    idx_t k = 1;
    while (1) {
        idx_t m = k + 3;                         /* binom(m,3) */
        idx_t binom_val = (m * (m - 1) * (m - 2)) / 6;
        if (binom_val >= n + 1) break;
        ++k;
    }

    /* Offset of n within the block of length k (1‑based) */
    idx_t prev = ((k + 2) * (k + 1) * k) / 6;      /* binom(k+2,3) */
    idx_t offset = n - (prev - 1);                /* subtract cumulative previous count */

    /* Build the number digit by digit */
    seq_t value = 0;
    idx_t cur_min = 1;                            /* smallest allowed digit so far */
    for (idx_t pos = 1; pos <= k; ++pos) {
        idx_t rem = k - pos;                      /* remaining positions after this one */
        for (idx_t d = cur_min; d <= 3; ++d) {
            idx_t m = 4 - d;                      /* number of digit choices ≥ d */
            idx_t count = binom_small(rem + m - 1, m - 1);
            if (offset <= count) {
                value = value * (seq_t)10 + (seq_t)d;
                cur_min = d;
                break;
            } else {
                offset -= count;
            }
        }
    }
    return value;
}
