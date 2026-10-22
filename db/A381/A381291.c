/*
 * A381291  Number of subsets of 8 integers between 1 and n such that their
 *          sum is 0 modulo n.
 * https://oeis.org/A381291
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( C(n,8) + [n even]·C(n/2,4) + 2·[n divisible by
 *            4]·C(n/4,2) - [n divisible by 8]·(n/2) ) / n where [condition]
 *            is 1 if true else 0. This follows from the roots‑of‑unity filter
 *            and the observation that only m ∈ {1,2,4,8} contribute to the
 *            eighth elementary symmetric sum of the nth roots of...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide – the division is exact at each step */
        res = res * (seq_t)(n - (k - i));
        res /= (seq_t)i;
    }
    return res;
}
 
seq_t a(idx_t n)
{
    if (n < 9) return 0;   /* values below offset are irrelevant */
 
    seq_t num = binom(n, 8);
    if ((n & 1) == 0)              /* n even            */
        num += binom(n >> 1, 4);
    if ((n & 3) == 0)              /* n divisible by 4   */
        num += (seq_t)2 * binom(n >> 2, 2);
    if ((n & 7) == 0)              /* n divisible by 8   */
        num -= (seq_t)(n >> 1);   /* subtract n/2       */
    return num / n;
}
