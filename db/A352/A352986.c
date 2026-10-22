/*
 * A352986  a(n) = Sum_{k=0..floor(n/3)} k^(3*(n-3*k)).
 * https://oeis.org/A352986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} k^(3*(n-3*k))
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is uint64_t;  k also */
    idx_t max_k = n / 3;               /* floor(n/3) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t base = (seq_t)k;
        idx_t m = n - 3 * k;
        seq_t exp = (seq_t)(3 * m);

        seq_t term;
        if (k == 0 && exp == 0) {     /* 0^0 is defined as 1 for this sequence */
            term = (seq_t)1;
        } else {
            term = big_pow(base, exp);
        }
        sum += term;
    }
    return sum;
}
