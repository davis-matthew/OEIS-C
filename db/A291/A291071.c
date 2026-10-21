/*
 * A291071  Number of 6 X n binary matrices that are "primitive"; that is,
 *          they cannot be expressed as a "tiling" by a smaller matrix.
 * https://oeis.org/A291071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r|6} μ(6/r) * Σ_{c|n} μ(n/c) * 2^{r*c}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 6 divides 6 always, so r ranges over {1,2,3,6}.  μ(6/r) is
 *  μ(6)=1, μ(3)=-1, μ(2)=-1, μ(1)=1.  This function counts how many
 *  6×n binary matrices are “primitive”, i.e. have no smaller
 *  vertical or horizontal period. */

#include <stddef.h>
#include "../utils/pow2.h"

/* Compute Möbius μ(k) for 1 ≤ k ≤ 10⁴.  The implementation runs in
 * O(√k) time, which is fine for the divisor sizes that appear. */
static int mu_of(idx_t k)
{
    if (k == 1) return 1;
    int sign = 1;
    idx_t m = k;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            if (cnt > 1) return 0;   /* squared prime factor */
            sign = -sign;
        }
    }
    if (m > 1) sign = -sign;       /* remaining prime factor */
    return sign;
}

/* Return the n-th term of the sequence.  Undefined for n < 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* formula is undefined for n=0 */

    /* Divisors of 6 and the corresponding μ(6/r). */
    static const idx_t R[4]   = {1, 2, 3, 6};
    static const int  MU_R[4] = { 1, -1, -1,  1};

    /*  Compute all divisors of n.  For n ≤ 10⁴ there are at most 128. */
    idx_t divs[128];
    size_t div_cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[div_cnt++] = d;
            if (d * d != n) divs[div_cnt++] = n / d;
        }
    }

    /* Accumulate positive and negative contributions separately to
     * avoid underflow on unsigned seq_t types. */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (size_t i = 0; i < 4; ++i) {
        idx_t r = R[i];
        int mu_r = MU_R[i];
        if (mu_r == 0) continue;   /* never happens */

        for (size_t j = 0; j < div_cnt; ++j) {
            idx_t c = divs[j];
            int mu_nc = mu_of(n / c);
            if (mu_nc == 0) continue;

            int sign = mu_r * mu_nc;
            seq_t term = pow2(r * c);   /* 2^{r*c} */

            if (sign > 0)
                pos_sum += term;
            else
                neg_sum += term;
        }
    }

    return pos_sum - neg_sum;
}
