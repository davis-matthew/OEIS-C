/*
 * A395546  Number of satisfiable 3-SAT formulas with n variables and 5
 *          clauses in the multiset clause model.
 * https://oeis.org/A395546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = binomial(binomial(2*n+2,3)+4,5) - (129332*n^6 -
 *            2237100*n^5 + 16427761*n^4 - 63136975*n^3 + 130884126*n^2 -
 *            135887564*n + 53820480)/3 (cf. OEIS A395546)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return binomial(n,k) for small k (k <= 5) using seq_t arithmetic.
   n may be larger than idx_t, but k is small. */
static seq_t binom_small(seq_t n, idx_t k)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        res *= (n - (seq_t)i);          /* multiply by (n-i)           */
    for (idx_t d = 2; d <= k; ++d)       /* divide by d!                 */
        res /= (seq_t)d;
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute binomial(2*n+2,3) */
    idx_t inner_n = 2 * n + 2;                         /* inner argument fits idx_t */
    seq_t inner_binom = binom_small((seq_t)inner_n, 3);

    /* Add 4 and compute outer binomial */
    seq_t inner_plus = inner_binom + (seq_t)4;
    seq_t total_binom = binom_small(inner_plus, 5);

    /* Compute the polynomial for A396491(n) */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t n4 = n3 * n_seq;
    seq_t n5 = n4 * n_seq;
    seq_t n6 = n5 * n_seq;

    seq_t poly_num =
          (seq_t)129332 * n6
        - (seq_t)2237100 * n5
        + (seq_t)16427761 * n4
        - (seq_t)63136975 * n3
        + (seq_t)130884126 * n2
        - (seq_t)135887564 * n_seq
        + (seq_t)53820480;

    seq_t sub = poly_num / (seq_t)3;

    return total_binom - sub;
}
