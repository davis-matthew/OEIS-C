/*
 * A258330  Inverse permutation to A258329.
 * https://oeis.org/A258330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let d = number of decimal digits of n. let t = n*7. let r = t
 *            mod 10^d. if r >= 10^(d-1) then a(n)=r else a(n)=t
 * Range:     u8 n <= 36, u16 n <= 9998, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external helper functions are required. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the number of decimal digits of n (n >= 1). */
    idx_t len = 0;
    idx_t tmp = n;
    do {
        ++len;
        tmp /= 10U;
    } while (tmp);

    /* Compute 10^len as seq_t. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < len; ++i) {
        pow10 *= (seq_t)10;
    }

    /* Multiply by 7. */
    seq_t t = (seq_t)n * (seq_t)7;

    /* Compute remainder modulo 10^len. */
    seq_t r = t % pow10;

    /* Threshold = 10^(len-1). */
    seq_t threshold = pow10 / (seq_t)10;

    if (r >= threshold) {
        return r;
    } else {
        return t;
    }
}
