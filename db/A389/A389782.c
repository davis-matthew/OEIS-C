/*
 * A389782  Expansion of e.g.f. exp( -LambertW(-x*(1+x)) ) / (1+x).
 * https://oeis.org/A389782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (k+1)^{k‑1} * binomial(k‑1, n‑k) / k!
 *            (the k = 0 term is (-1)^n · n!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* auxiliary: factorial of n */
static seq_t factorial(idx_t n) {
    seq_t r = 1;
    for (idx_t i = 1; i <= n; ++i) {
        r *= (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n) {
    /* special case n = 0 */
    if (n == 0)
        return 1;

    /* compute n! once */
    seq_t fact_n = factorial(n);

    /* separation of the k = 0 contribution (+ sign for even, – for odd) */
    seq_t sum = 0;
    if ((n & 1) == 0)                     /* n even */
        sum = fact_n;
    else                                   /* n odd */
        sum = 0;

    /* k runs from n down to ceil((n+1)/2) */
    idx_t k_min = (n + 2) / 2;            /* ceil((n+1)/2) */
    seq_t factor = 1;                     /* product k+1 .. n */

    for (idx_t k = n;; --k) {
        /* binomial coefficient */
        seq_t bin = binomial(k - 1, n - k);
        if (bin != 0) {
            /* (k+1)^(k-1) */
            seq_t pow_val = big_pow((seq_t)(k + 1), (seq_t)(k - 1));
            /* term = factor * pow_val * bin */
            seq_t term = pow_val;
            term *= bin;
            term *= factor;
            sum += term;
        }
        if (k == k_min)
            break;                         /* avoid underflow of unsigned k */
        factor *= k;                       /* prepare product for k-1 */
    }

    /* subtract the k = 0 term for odd n */
    if ((n & 1) != 0)
        sum -= fact_n;

    return sum;
}
