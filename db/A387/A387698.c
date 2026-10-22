/*
 * A387698  Irregular triangle read by rows in which row n is the maximum
 *          clique containing n in the gcd-graph of {1, 2, ..., n}. If more
 *          than one such clique exists, choose the lexicographically
 *          earliest.
 * https://oeis.org/A387698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term in the flattened irregular triangle where
 *            each row m consists of all multiples of the smallest prime
 *            factor of m (or {1} when m = 1). The sequence is generated row
 *            by row
 * Range:     u8 n <= 1691, u16 n <= 1691, u32 n <= 1691, u64 n <= 1691,
 *            u128 n <= 1691, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* smallest prime factor of n (naïve trial division) */
static uint64_t spf(uint64_t n)
{
    if (n <= 1) return n;           /* 1 is handled separately */
    if ((n & 1ULL) == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL) return d;
    return n;                       /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for 0, guard anyway */

    idx_t term_index = 0;           /* number of terms emitted so far */
    for (idx_t row = 1; ; ++row)   /* generate rows until target reached */
    {
        if (row == 1)               /* special case: row {1} */
        {
            ++term_index;
            if (term_index == n) return (seq_t)1;
            continue;
        }

        uint64_t p = spf((uint64_t)row);        /* smallest prime factor */
        idx_t step = (idx_t)p;
        for (idx_t val = step; val <= row; val += step)
        {
            ++term_index;
            if (term_index == n) return (seq_t)val;
        }
    }
}
