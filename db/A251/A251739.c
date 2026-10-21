/*
 * A251739  Smallest k such that n * sum(i=0..k, binomial(k,i) mod (n-1) ) <=
 *          2^n.
 * https://oeis.org/A251739
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that * n * Σ_{i=0..k} ( binomial(k,i)
 *            mod (n-1) ) ≤ 2^k
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;    /* undefined for n < 2 but harmless */

    const idx_t mod = n - 1;   /* n‑1 ≥ 1 */

    /* Search for the smallest k satisfying the inequality. */
    for (idx_t k = 1; ; ++k)
    {
        seq_t sum = 0;
        seq_t c   = 1;                  /* C(k,0) */

        /* i = 0 term */
        sum += c % (seq_t)mod;

        /* compute remaining binomial coefficients */
        for (idx_t i = 1; i <= k; ++i)
        {
            c = (c * (seq_t)(k - i + 1)) / (seq_t)i;  /* exact */
            sum += c % (seq_t)mod;
        }

        seq_t prod   = (seq_t)n * sum;
        seq_t pow2k  = pow2(k);          /* 2^k in seq_t */

        if (prod <= pow2k)
            return (seq_t)k;
    }
}
