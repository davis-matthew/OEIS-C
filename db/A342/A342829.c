/*
 * A342829  a(n) is the smallest number whose sum of digits is equal to the
 *          sum of digits of n.
 * https://oeis.org/A342829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest m such that digit_sum(m) = digit_sum(n)
 * Range:     u8 n <= 298, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t digit_sum_idx(idx_t x) {
    idx_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for offset 1, just return 0 */
    idx_t S = digit_sum_idx(n);      /* digit sum of n */
    if (S == 0) return 0;            /* for n = 0 only */
    idx_t q = S / 9;                 /* number of trailing 9's */
    unsigned int r = S % 9;          /* leading digit (0..8) */
    seq_t result = (seq_t)r;         /* start with leading digit */
    for (idx_t i = 0; i < q; ++i) {
        result = result * 10 + 9;    /* append a 9 each time */
    }
    return result;
}
