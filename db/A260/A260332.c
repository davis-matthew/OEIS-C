/*
 * A260332  Labelings of n diamond-shaped posets with 4 vertices per diamond
 *          where the labels follow the poset relations whose associated
 *          reading permutation avoids 231 in the classical sense.
 * https://oeis.org/A260332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} binomial(n,k) * binomial(4*n,k-1) *
 *            2^k (n > 0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* If seq_t is 64 (or 32, etc.) bits, use __int128 for intermediate
       arithmetic to avoid overflow of the intermediate sum.  For larger
       seq_t types (arbitrary precision) use the library binomial function
       and normal seq_t arithmetic. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        unsigned __int128 bin_nk = 1;               /* current binom(n,k)   */
        unsigned __int128 bin_4n_kminus1 = 1;       /* current binom(4n,k-1) */
        unsigned __int128 four_n = (unsigned __int128)4 * n;

        for (idx_t k = 1; k <= n; ++k) {
            /* update binom(n,k) */
            bin_nk = bin_nk * (unsigned __int128)(n - k + 1) /
                     (unsigned __int128)k;

            /* term = bin_nk * bin_4n_kminus1 * 2^k */
            unsigned __int128 term = bin_nk * bin_4n_kminus1;
            term <<= k;           /* multiply by 2^k */
            sum += term;

            /* prepare binom(4n,k) for next iteration */
            bin_4n_kminus1 =
                bin_4n_kminus1 *
                (four_n - (unsigned __int128)(k - 1)) /
                (unsigned __int128)k;
        }

        sum /= n;                /* divide by n, guaranteed integer */
        return (seq_t)sum;       /* fits in 64‑bit for all n with a(n) in 64‑bit */
    } else {
        /* Arbitrary-precision path */
        seq_t sum = 0;
        seq_t bin_nk = 1;               /* current binom(n,k)   */
        seq_t bin_4n_kminus1 = 1;       /* current binom(4n,k-1) */
        seq_t four_n = ((seq_t)4) * ((seq_t)n);

        for (idx_t k = 1; k <= n; ++k) {
            /* update binom(n,k) */
            bin_nk = bin_nk * ((seq_t)(n - k + 1)) / (seq_t)k;

            /* term = bin_nk * bin_4n_kminus1 * 2^k */
            seq_t term = bin_nk * bin_4n_kminus1;
            term = term * pow2(k);
            sum += term;

            /* prepare binom(4n,k) for next iteration */
            bin_4n_kminus1 =
                bin_4n_kminus1 * (four_n - (seq_t)(k - 1)) / (seq_t)k;
        }

        sum = sum / (seq_t)n;
        return sum;
    }
}
