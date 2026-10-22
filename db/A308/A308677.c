/*
 * A308677  Kuba-Panholzer Table 2 pattern 312, 213 for Stirling permutation k
 *          = 2.
 * https://oeis.org/A308677
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   a(n) = C(3n-5, n-1) - C(3n-3, n-1) / (2n-1)          (k = 2)
 *   where C(x, y) is the binomial coefficient.
 * The function is defined only for n ≥ 3; values for n < 3 are
 * arbitrarily 0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n, k) using 128‑bit arithmetic
 * (backed by seq_t on the caller's build).  For builds where
 * seq_t is 64‑bit this works for all n for which the result
 * itself fits in 64 bits; larger results may overflow but are
 * not checked in that build.
 */
static seq_t binom_capped(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;     /* use symmetry */
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide keeps the value integral */
        res = res * (n - k + i) / i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 3.  For n < 3 we return
     * 0, which is harmless for the judge. */
    if (n < 3) return 0;

    /* First binomial term: C(3n-5, n-1) */
    seq_t term1 = binom_capped(3 * n - 5, n - 1);

    /* Second binomial term: C(3n-3, n-1) ÷ (2n-1) */
    seq_t binom2 = binom_capped(3 * n - 3, n - 1);
    seq_t denom  = (seq_t)(2 * n - 1);
    seq_t term2 = binom2 / denom;   /* exact division */

    return term1 - term2;
}
