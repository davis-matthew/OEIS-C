/*
 * A384868  a(n) = Sum_{i=1...|b|} i*(-1)^b_i where b is the lexicographically
 *          n-th binary string.
 * https://oeis.org/A384868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, look, easy
 * Formula:   a(n) = sum_{i=1}^{len(n)} i * (-1)^{b_i} where b is the
 *            lexicographically n-th binary string (empty string for n = 0)
 * Range:     i8 n <= 8909, i16 n <= 8909, i32 n <= 8909, i64 n <= 8909,
 *            i128 n <= 8909, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Determine the length len of the binary string.
       It satisfies 2^len ≤ n+1 < 2^{len+1} */
    idx_t len = 0;
    idx_t tmp = n + 1;
    while (tmp > 1) {
        tmp >>= 1;
        len++;
    }

    /* Compute the offset of the string among all strings of this length.
       The preceding count is 2^len - 1. */
    idx_t offset = n - (((idx_t)1 << len) - 1);

    seq_t sum = 0;
    for (idx_t i = 1; i <= len; ++i) {
        /* Extract the i-th bit from the left (most significant). */
        int bit = (offset >> (len - i)) & 1;
        if (bit == 0)
            sum += i;
        else
            sum -= i;
    }
    return sum;
}
