/*
 * A343255  a(n) is obtained by replacing 2's by n's in the hereditary base-2
 *          expansion of n.
 * https://oeis.org/A343255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A342707(n, n) // hereditary base‑2 expansion of n, with
 *            each 2 replaced by n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static seq_t eval_h(idx_t m, seq_t base)
{
    /* Hereditary evaluation of integer m with base 'base'.
       Base case: m==0 -> 0 (so that base^0 = 1 later),
       m==1 -> 1.
    */
    if (m == 0)
        return (seq_t)0;
    if (m == 1)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t pos = 0;
    idx_t t = m;
    while (t)
    {
        if (t & 1ULL)
        {
            seq_t exp = eval_h(pos, base);   // value of the exponent pos
            seq_t term = big_pow(base, exp); // base^exp
            sum += term;
        }
        t >>= 1;
        pos++;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    /* Evaluate the hereditary base‑2 expansion of n,
       replacing each 2 by n. */
    return eval_h(n, (seq_t)n);
}
