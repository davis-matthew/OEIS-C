/*
 * A253604  a(n) = (n^(n-1)+1)^n.
 * https://oeis.org/A253604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^(n-1)+1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is defined; for n == 0 return 0 (unused) */
    if (n == 0)
        return 0;

    /* n^(n-1) */
    seq_t pow_term = big_pow((seq_t)n, (seq_t)(n - 1));

    /* n^(n-1) + 1 */
    seq_t sum_term = pow_term + (seq_t)1;

    /* (n^(n-1) + 1)^n */
    return big_pow(sum_term, (seq_t)n);
}
