/*
 * A370528  Number of permutations of [n] having exactly two adjacent
 *          3-cycles.
 * https://oeis.org/A370528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Sum_{k=0..floor(n/3)-2} (-1)^k * (n-2k-4)! / k!
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 16, u64 n <= 24, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 6 there are no permutations with two adjacent 3‑cycles */
    if (n < 6U)
        return (seq_t)0;

    /* kmax = floor(n/3) - 2; if floor(n/3) < 2 there is no term */
    if (n / 3U < 2U)
        return (seq_t)0;

    idx_t kmax = n / 3U - 2U;

    /* Compute (n-4)! as the starting term for k = 0 */
    seq_t term = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n - 4U; ++i)
        term = term * (seq_t)i;

    seq_t sum_even = (seq_t)0;
    seq_t sum_odd  = (seq_t)0;
    seq_t current  = term;          /* term for k = 0 */

    for (idx_t k = 0U; k <= kmax; ++k)
    {
        if (k == 0U)
            current = term;
        else
        {
            /* Update current from k-1 to k:
               current = current / (k * (n-2k-2) * (n-2k-3)) */
            seq_t denom = (seq_t)k *
                          (seq_t)(n - 2U * k - 2U) *
                          (seq_t)(n - 2U * k - 3U);
            current = current / denom;
        }

        if ((k & 1U) == 0U)
            sum_even += current;
        else
            sum_odd  += current;
    }

    /* a(n) = (sum_even - sum_odd) / 2 */
    seq_t diff = sum_even - sum_odd;
    return diff / (seq_t)2;
}
