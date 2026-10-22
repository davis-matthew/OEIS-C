/*
 * A381289  Number of subsets of 6 integers between 1 and n such that their
 *          sum is 0 modulo n.
 * https://oeis.org/A381289
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (C(n,6) - [n even] * C(n/2,3) + 2*[n divisible by 3] *
 *            C(n/3,2) * - 2*[n divisible by 6] * (n/6)) / n * for n ≥ 7,
 *            where C(a,b) is the binomial coefficient
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_small(idx_t N, int k)
{
    seq_t r = 1;
    for (int i = 1; i <= k; ++i) {
        r = r * (seq_t)(N - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 7) return 0;            /* behavior below offset is undefined */

    seq_t term1 = binom_small(n, 6);

    seq_t term2 = 0;
    if ((n & 1ULL) == 0)            /* n even */
        term2 = binom_small(n / 2, 3);

    seq_t term3 = 0;
    if (n % 3ULL == 0)              /* n divisible by 3 */
        term3 = binom_small(n / 3, 2) * 2;

    seq_t term4 = 0;
    if (n % 6ULL == 0)              /* n divisible by 6 */
        term4 = (seq_t)(n / 6) * 2;

    seq_t numerator = term1 - term2 + term3 - term4;
    return numerator / (seq_t)n;
}
