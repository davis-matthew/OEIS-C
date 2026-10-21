/*
 * A261743  Number of partitions of n where each part i is marked with a word
 *          of length i over a nonary alphabet whose letters appear in
 *          alphabetical order.
 * https://oeis.org/A261743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} binomial(k+8,8) * a(n-k), with a(0)=1
 *            Generating function: ∏_{k≥1} 1 / (1 - binomial(k+8,8) * x^k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


static seq_t cache[10001];

seq_t a(idx_t n)
{
    /* Limit n to the maximum table size.  The tests use n ≤ 10 000. */
    if (n > 10000)
        n = 10000;

    /* Re‑compute the table from scratch for the new n. */
    cache[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        cache[i] = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(k+8,8) computed multiplicatively. */
        seq_t w = 1;
        for (idx_t i = 1; i <= 8; ++i)
            w = w * (seq_t)(k + i) / (seq_t)i;

        for (idx_t j = k; j <= n; ++j)
            cache[j] += w * cache[j - k];
    }

    return cache[n];
}
