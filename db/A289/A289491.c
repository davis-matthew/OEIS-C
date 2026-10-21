/*
 * A289491  a(n) = denominator of 1/(1 + 1/(1 + 2/(1 + ... (1 + n)))).
 * https://oeis.org/A289491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of 1/(1 + 1/(1 + 2/(1 + ... (1 + n))))
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"   /* gcd(seq_t, seq_t) is provided in the utils folder. */

seq_t a(idx_t n) {
    /* Start from the innermost fraction T_n = 1 + n (numerator) / 1 (denominator). */
    seq_t p = (seq_t)n + 1;   /* numerator of the current T_i */
    seq_t q = 1;             /* denominator of the current T_i */

    /* Work outwards, from i = n-1 down to 1. */
    for (idx_t i = n - 1; i >= 1; --i) {
        seq_t new_num = p + (seq_t)i * q;   /* numerator after adding i/T_{i+1} */
        seq_t new_den = p;                  /* denominator becomes the previous numerator */
        seq_t g = gcd(new_num, new_den);    /* reduce the fraction */
        p = new_num / g;                    /* new numerator after reduction */
        q = new_den / g;                    /* new denominator after reduction */
    }

    /* After the loop, T_1 = p/q.  The desired denominator a(n) is the numerator of T_1. */
    return p;
}
