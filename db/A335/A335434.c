/*
 * A335434  Number of separable factorizations of n into factors > 1.
 * https://oeis.org/A335434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of unordered factorizations of n into integers >1
   such that the multiset of factors can be permuted with no two equal
   adjacent parts (i.e., the multiset is separable).  The routine
   enumerates all multiplicative partitions in non‑decreasing order
   and checks the separability condition by verifying that the largest
   multiplicity of any factor is <= (total factors + 1)/2. */

#include <stdint.h>

static seq_t count_factorizations(idx_t rem, idx_t min_factor,
                                  uint64_t *factors, idx_t len)
{
    seq_t total = 0;

    /* try all possible split factors from min_factor up to sqrt(rem) */
    for (idx_t f = min_factor; f * f <= rem; ++f)
    {
        if (rem % f == 0)
        {
            factors[len] = f;
            total += count_factorizations(rem / f, f, factors, len + 1);
        }
    }

    /* use rem itself as the last factor (terminating leaf) */
    if (rem >= min_factor)
    {
        factors[len] = rem;
        idx_t total_factors = len + 1;

        /* compute maximum multiplicity in the factor multiset */
        idx_t maxfreq = 0;
        idx_t i = 0;
        while (i <= len)
        {
            idx_t j = i;
            while (j <= len && factors[j] == factors[i]) ++j;
            idx_t cnt = j - i;
            if (cnt > maxfreq) maxfreq = cnt;
            i = j;
        }

        /* separable iff maxfreq <= (total_factors + 1)/2   */
        if (maxfreq <= (total_factors + 1) / 2)
            total += (seq_t)1;
    }

    return total;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 by convention */

    /* buffer for current factor list; depth never exceeds log2(n) <= 64 for n ≤ 10⁶ */
    static uint64_t factors[64];
    return count_factorizations(n, (idx_t)2, factors, (idx_t)0);
}
