/*
 * A372852  a(n) is the total number of runs of ascents over all flattened
 *          Catalan words of length n.
 * https://oeis.org/A372852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n-1) + 1)*(n + 1)/4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* 3^(n-1) in arbitrary precision */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));
    seq_t val   = pow3 + 1;                     /* 3^(n-1) + 1      */
    seq_t mul   = (seq_t)(n + 1);                /* n + 1            */
    return (val * mul) / 4;                     /* integer division */
}
