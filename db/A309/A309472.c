/*
 * A309472  a(n) = n^n - n * n!.
 * https://oeis.org/A309472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - n * n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t base = (seq_t)n;
    seq_t pow = big_pow(base, (seq_t)n);     /* n^n */
    seq_t fact = factorial(n);              /* n! */
    seq_t prod = (seq_t)n * fact;           /* n * n! */
    return pow - prod;
}
