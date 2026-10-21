/*
 * A295552  a(n) = n! * [x^n] exp(x*exp(n*x)).
 * https://oeis.org/A295552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n,k) * (n*k)^{n-k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             (OEIS A295552)                                  */
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute a(n) for OEIS A295552 */
seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t comb = 1;                    /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update binomial(n,k) = binomial(n,k-1) * (n-k+1) / k */
            comb = (comb * ((seq_t)(n - k + 1))) / (seq_t)k;
        }
        if (n == 0 && k == 0) {        /* handle 0^0 = 1 */
            /* base = 0, exp = 0 -> power = 1 */
            sum += comb;              /* since term = 1 */
            continue;
        }
        /* Base of the power: (n*k) */
        uint64_t base_val = (uint64_t)n * (uint64_t)k;
        seq_t base = (seq_t)base_val;
        seq_t exp  = (seq_t)(n - k);
        seq_t pow_val = big_pow(base, exp);
        seq_t term = comb * pow_val;
        sum += term;
    }
    return sum;
}
