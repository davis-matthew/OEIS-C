/*
 * A380020  Expansion of e.g.f. 1/sqrt(exp(-2*x) - 2*x*exp(-x)).
 * https://oeis.org/A380020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(n,k) * Π_{j=1..k}(2j-1) * (k+1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* small exponentiation: base^exp, exp ≤ n */
static seq_t pow_seq_idx(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t prod_odds = 1;   /* Π_{j=1..k}(2j-1)  */
    seq_t binom     = 1;   /* C(n,k)            */
    seq_t sum       = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t power = pow_seq_idx((seq_t)(k + 1), n - k);
        seq_t term  = binom * prod_odds * power;
        sum += term;

        if (k < n) {
            /* next prod_odds: multiply by (2*(k+1)-1) = 2k+1 */
            prod_odds *= (seq_t)(2 * k + 1);
            /* next binom: binom * (n-k)/(k+1) */
            binom *= (seq_t)(n - k);
            binom /= (seq_t)(k + 1);
        }
    }
    return sum;
}
