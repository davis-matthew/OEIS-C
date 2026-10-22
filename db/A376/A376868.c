/*
 * A376868  a(n) = n! * 4^binomial(n, 2).
 * https://oeis.org/A376868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * 4^binomial(n, 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    seq_t fact = factorial(n);
    seq_t exp  = binomial(n, 2);
    seq_t pow4 = pow_seq_large((seq_t)4, exp);
    return fact * pow4;
}
