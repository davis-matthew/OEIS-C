/*
 * A300655  a(n) is the length of the longest contiguous block of ones in the
 *          binary expansion of 1/n.
 * https://oeis.org/A300655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of longest contiguous block of 1's in binary *
 *            expansion of 1/n. The sequence satisfies a(2*n) = a(n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n < 1) return (seq_t)0;

    /* Remove all factors of 2, as a(2*n) = a(n) */
    idx_t m = n;
    while ((m & 1U) == 0) m >>= 1;

    /* For n = 1 (after removing 2's) the binary expansion is '1.0...' */
    if (m == 1) return (seq_t)1;

    /* Generate one full period of the binary expansion of 1/m */
    idx_t rem = 1;
    /* the period length never exceeds m-1 for odd m */
    idx_t *bits = (idx_t *)malloc(sizeof(idx_t) * (m));
    if (!bits) return (seq_t)0;  /* allocation failed */
    idx_t t = 0;
    do {
        rem <<= 1;                      /* rem = rem * 2 */
        idx_t bit = rem / m;            /* integer part of division  */
        bits[t++] = bit;
        rem = rem % m;                  /* new remainder */
    } while (rem != 1);

    /* Compute longest run of consecutive 1's in the period */
    seq_t max_run = 0;
    seq_t cur_run = 0;
    for (idx_t i = 0; i < t; ++i) {
        if (bits[i]) {
            ++cur_run;
            if (cur_run > max_run) max_run = cur_run;
        } else {
            cur_run = 0;
        }
    }

    /* Compute prefix and suffix lengths of 1's for possible wrap-around */
    seq_t prefix = 0, suffix = 0;
    for (idx_t i = 0; i < t && bits[i]; ++i) prefix++;
    for (idx_t i = t; i > 0 && bits[i - 1]; --i) suffix++;

    /* All bits are 1, entire period is a run */
    seq_t result;
    if (prefix == t) {
        result = (seq_t)t;
    } else {
        seq_t wrap = prefix + suffix;
        result = (wrap > max_run) ? wrap : max_run;
    }

    free(bits);
    return result;
}
