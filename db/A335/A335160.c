/*
 * A335160  a(n) = nim-product of 2^3 and 2^n.
 * https://oeis.org/A335160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nim-product of 2^3 and 2^n Recurrence: a(n) = 16 * a(n-4)
 *            for n > 3 Initial values: a(0) = 8, a(1) = 12, a(2) = 11, a(3) =
 *            13
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    static const seq_t base[4] = { (seq_t)8, (seq_t)12, (seq_t)11, (seq_t)13 };
    if (n < 4)
        return base[n];
    /* a(n) = 16^(n/4) * a(n%4) */
    idx_t k = n / 4;
    idx_t r = n % 4;
    seq_t power = seq_pow((seq_t)16, k);
    return seq_mul(power, base[r]);
}
