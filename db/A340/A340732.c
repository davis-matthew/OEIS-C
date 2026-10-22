/*
 * A340732  Number of partitions of n into 4 parts such that the product of
 *          the smallest and largest parts is equal to the product of the
 *          middle two parts.
 * https://oeis.org/A340732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/4⌋} Σ_{j=k..⌊(n−k)/3⌋} [ (k*(n−k−j)) mod (j+k)
 *            = 0 * and i = (k*(n−k−j))/(j+k) satisfies i ≥ j ≥ k and * l =
 *            n−i−j−k ≥ i ] * with l the largest part. This counts 4‑part
 *            partitions * k ≤ j ≤ i ≤ l with k·l = j·i
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 4) return 0;
    for (idx_t k = 1; k <= n / 4; ++k) {
        for (idx_t j = k; j <= (n - k) / 3; ++j) {
            idx_t rem = n - k - j;              /* sum of i and l */
            idx_t num = k * rem;                /* numerator for i */
            idx_t denom = j + k;                /* denominator for i */
            if (num % denom != 0) continue;     /* i must be integer */
            idx_t i = num / denom;              /* middle part */
            if (i < j) continue;                /* maintain order k ≤ j ≤ i */
            idx_t l = n - i - j - k;            /* largest part */
            if (l < i) continue;                /* maintain order i ≤ l */
            count += 1;
        }
    }
    return count;
}
