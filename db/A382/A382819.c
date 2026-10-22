/*
 * A382819  Number of Grassmannian permutations on [n] of order dividing 3.
 * https://oeis.org/A382819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(floor(n/3)+3,3) + binomial(floor((n-1)/3)+3,3) *
 *            + binomial(floor((n-2)/3)+3,3) - n
 * Range:     u8 n <= 14, u16 n <= 147, u32 n <= 5269, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute binomial(m,3) for m >= 0 */
static seq_t choose3(idx_t m)
{
    if (m < 3) return 0;
    seq_t res = (seq_t)m * ((seq_t)m - 1) * ((seq_t)m - 2);
    return res / 6;
}

seq_t a(idx_t n)
{
    int64_t kn = (int64_t)n;                     /* signed for negative cases */
    int64_t k0 = kn / 3;                         /* floor(n/3)        */
    int64_t k1 = (kn >= 1) ? ((kn - 1) / 3) : -1; /* floor((n-1)/3)   */
    int64_t k2 = (kn >= 2) ? ((kn - 2) / 3) : -1; /* floor((n-2)/3)   */

    seq_t m0 = (seq_t)(k0 + 3);          /* arguments to binomial */
    seq_t m1 = (seq_t)(k1 + 3);
    seq_t m2 = (seq_t)(k2 + 3);

    seq_t sum = choose3((idx_t)m0) + choose3((idx_t)m1) + choose3((idx_t)m2);
    return sum - (seq_t)n;
}
