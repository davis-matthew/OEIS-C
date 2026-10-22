/*
 * A350145  a(n) = Sum_{k=1..n} floor(n/(2*k-1))^n.
 * https://oeis.org/A350145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/(2*k-1))^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    seq_t sum = 0;
    seq_t exp = (seq_t)n;
    idx_t prev_t = 0;
    idx_t count = 0;

    for (idx_t m = 1; m <= n; m += 2) {
        idx_t t = n / m;            /* floor division */
        if (t == 0) break;          /* further terms are zero */

        if (t == prev_t) {
            ++count;
        } else {
            if (prev_t != 0) {
                seq_t term = big_pow((seq_t)prev_t, exp) * (seq_t)count;
                sum += term;
            }
            prev_t = t;
            count = 1;
        }
    }

    if (prev_t != 0) {
        sum += big_pow((seq_t)prev_t, exp) * (seq_t)count;
    }

    return sum;
}
