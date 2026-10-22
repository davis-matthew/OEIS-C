/*
 * A394707  Coefficient of x^n in the expansion of ( (1+x)^2 * (1+x^2) )^n.
 * https://oeis.org/A394707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n,k) * binomial(2n, n-2k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k_limit = n / 2;

    seq_t c_nk = 1;                     /* binomial(n,0) */
    idx_t two_n = n << 1;
    idx_t r = n;                        /* r = n-2k, initial k=0 */
    seq_t c_2n_r = binomial(two_n, r);  /* binomial(2n, n) */

    for (idx_t k = 0; k <= k_limit; ++k) {
        sum += c_nk * c_2n_r;

        if (k == k_limit)
            break;                      /* last term reached */

        /* Update C(n, k+1): C(n,k+1)=C(n,k)*(n-k)/(k+1) */
        c_nk = (c_nk * (n - k)) / (k + 1);

        /* Update C(2n, r-2) from C(2n, r) */
        idx_t r_old = r;            /* current r before decrement */
        r -= 2;                     /* new r for next iteration */

        /* C(2n, r_new) = C(2n, r_old) * r_old * (r_old-1) / ((2n - r_old + 1)*(2n - r_old + 2)) */
        seq_t numer = (seq_t)r_old * (seq_t)(r_old - 1);
        seq_t denom = (seq_t)(two_n - r_old + 1) * (seq_t)(two_n - r_old + 2);
        c_2n_r = (c_2n_r * numer) / denom;
    }

    return sum;
}
