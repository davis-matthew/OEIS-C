/*
 * A281706  Number of sets of three positive numbers <= n whose sum is a
 *          square.
 * https://oeis.org/A281706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ 1 ≤ i < j < k ≤ n : i + j + k is a perfect square }
 * Range:     u8 n <= 27, u16 n <= 243, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* -------------------------------------------------------------
 * Count unordered triples (i,j,k) with 1 ≤ i < j < k ≤ n
 * whose sum is a perfect square.
 *
 * The algorithm iterates over all squares S = t² ≤ 3·n.
 * For each square it counts all i (1..n) and admissible j
 * satisfying i < j < k = S – i – j, k ≤ n.
 * This takes O(n·sqrt(n)) time, about 1.7·10⁶ iterations for
 * n = 10⁴, well below the time limit.
 * ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 3) return 0;        /* no triples possible */

    seq_t total = 0;             /* result accumulator */

    /* Iterate over all squares S = t² that could be the sum of a triple. */
    /* The maximum possible sum is 3·n, so we only need t up to sqrt(3·n). */
    for (idx_t t = 1; ; ++t)
    {
        idx_t S = t * t;
        if (S > 3 * n) break;   /* no larger squares are relevant */

        /* For fixed S, count all triples (i,j,k) such that:
         *   1 ≤ i < j < k ≤ n
         *   i + j + k = S
         */
        for (idx_t i = 1; i <= n; ++i)
        {
            /* Lower bound for j:
             *   j ≥ i + 1          (since i < j)
             *   j ≥ S - i - n      (since k = S - i - j ≤ n)
             */
            idx_t j_min = i + 1;
            if (S > i + n) {
                idx_t tmp = S - i - n;
                if (tmp > j_min) j_min = tmp;
            }

            /* Upper bound for j:
             *   j < (S - i)/2     (since k > j)
             *   j ≤ n - 1         (since k ≥ j+1 and k ≤ n)
             */
            idx_t j_max;
            if (S <= i) {
                j_max = 0;        /* no solution if S ≤ i */
            } else {
                j_max = (S - i - 1) / 2;
                if (j_max > n - 1) j_max = n - 1;
            }

            if (j_min <= j_max) {
                /* Number of admissible j values for this i and S. */
                total += (seq_t)(j_max - j_min + 1);
            }
        }
    }

    return total;
}
