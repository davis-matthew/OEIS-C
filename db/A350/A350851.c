/*
 * A350851  Cumulative sums of the first ceiling(n/2)+1 elements of rows 0 to
 *          n in Pascal's triangle.
 * https://oeis.org/A350851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0}^n ∑_{j=0}^{⌈k/2⌉} binom(k,j)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute binomial coefficient C(n,k)  (0 ≤ k ≤ n) */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n - k)           /* use symmetry to minimise multiplications */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow2 = 1;   /* 2^0 */
    sum += 1;         /* term for k = 0 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t pow2_prev = pow2;   /* 2^(k-1) */
        pow2 = pow2 * 2;          /* 2^k */

        if ((k & 1) == 0) {       /* k even */
            idx_t r = k / 2;
            seq_t center = binom(k, r);
            sum += (pow2 + center) / 2;
        } else {                  /* k odd */
            idx_t r = (k + 1) / 2;
            seq_t center = binom(k, r);
            sum += pow2_prev + center;
        }
    }
    return sum;
}
