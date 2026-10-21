/*
 * A251696  a(n) = (4*n+1) * (5*n+1)^(n-2) * 6^n.
 * https://oeis.org/A251696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+1) * (5*n+1)^(n-2) * 6^n (for n >= 2); special
 *            cases: a(0) = 1, a(1) = 5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* 6^n */
    seq_t p6 = seq_pow((seq_t)6, n);

    /* (5*n+1)^(n-2) */
    seq_t base = (seq_t)(5 * n + 1);
    seq_t exp  = (seq_t)(n - 2);
    seq_t pow5 = big_pow(base, exp);

    /* (4*n+1) * pow5 */
    seq_t tem = seq_mul((seq_t)(4 * n + 1), pow5);

    /* final result */
    return seq_mul(tem, p6);
}
