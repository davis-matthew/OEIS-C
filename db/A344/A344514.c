/*
 * A344514  a(n) = Sum_{k=1..n} floor(n/k^2)^k.
 * https://oeis.org/A344514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k^2)^k
 * Range:     u8 n <= 44, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k)
    {
        /* Stop when k^2 exceeds n, since floor(n/k^2) will be zero afterwards */
        if (k * k > n)
            break;

        idx_t base = n / (k * k);
        if (base == 0)
            break;   /* further terms are zero as well */

        seq_t term = big_pow((seq_t)base, (seq_t)k);
        sum += term;
    }
    return sum;
}
