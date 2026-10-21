/*
 * A241865  Number of compositions of n such that the smallest part has
 *          multiplicity five.
 * https://oeis.org/A241865
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{s=1}^{⌊n/5⌋} * [ (n - 5s == 0) ? 1 * :
 *            Σ_{k=1}^{⌊(n-5s)/(s+1)⌋} * binom(n-5s - k·(s+1) + k - 1, k-1) *
 *            * binom(k+5, 5) ] * * The outer sum iterates over the possible
 *            least part s of the * composition. For each s the remainder m =
 *            n-5s must be split into * parts strictly larger than s....
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 37, u64 n <= 76, u128 n <= 161,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;          /* undefined below offset */

    seq_t total = 0;
    idx_t max_s = n / 5;                  /* largest possible least part */

    for (idx_t s = 1; s <= max_s; ++s)
    {
        idx_t m = n - 5 * s;              /* remainder after the five s's */
        if (m == 0)                      /* only five s's are present */
        {
            total += 1;
            continue;
        }

        idx_t t = s + 1;                  /* lower bound for the other parts */
        idx_t max_k = m / t;              /* maximum number of parts > s */

        for (idx_t k = 1; k <= max_k; ++k)
        {
            /* binomial for the ordered splitting of the remainder */
            idx_t a_bin = m - k * t + k - 1;
            idx_t b_bin = k - 1;
            seq_t ways_part = binom(a_bin, b_bin);

            /* interleavings with the 5 copies of s */
            seq_t ways_inter = binom(k + 5, 5);

            total += ways_part * ways_inter;
        }
    }

    return total;
}
