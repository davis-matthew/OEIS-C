/*
 * A363664  a(n) = Sum_{d|n} (n/d)^(n-n/d) * binomial(d+n-1,n).
 * https://oeis.org/A363664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(n-n/d) * binomial(d + n - 1, n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A363664 with arbitrary‑precision or fixed‑size seq_t */

#include "../utils/binom.h"    /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/seq_pow.h"  /* seq_t seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Sequence defined for n ≥ 1; return 0 for n == 0 for safety. */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        /* divisor d */
        idx_t k1 = n / d;              /* k = n/d   */
        idx_t exp1 = n - k1;           /* n - k     */
        seq_t part1 = seq_pow((seq_t)k1, exp1);
        seq_t bin1  = binom(d + n - 1, n);
        sum += part1 * bin1;

        /* complementary divisor n/d (if distinct) */
        idx_t d2 = n / d;
        if (d2 != d)
        {
            idx_t k2 = n / d2;             /* k = d     */
            idx_t exp2 = n - k2;           /* n - d     */
            seq_t part2 = seq_pow((seq_t)k2, exp2);
            seq_t bin2  = binom(d2 + n - 1, n);
            sum += part2 * bin2;
        }
    }

    return sum;
}
