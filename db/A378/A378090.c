/*
 * A378090  E.g.f. satisfies A(x) = exp(x * (1-x)^2 * A(x)) / (1-x)^3.
 * https://oeis.org/A378090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(n+2,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code evaluates the sum directly, using integer arithmetic only. */

#include "../utils/pow_seq.h"
#include <stdbool.h>

/* Helper to compute powers of a small integer base to an integral exponent. */
static seq_t small_pow(int base, idx_t exp)
{
    seq_t res = 1;
    seq_t b = (seq_t)base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1)
            res *= b;
        if (e >>= 1)
            b *= b;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;  /* a(0) = 1 */

    seq_t sum = 0;

    /* ratio = n! / k!   (initially k = n, so ratio = 1)   */
    seq_t ratio = 1;

    /* binom = binomial(n+2, k+2)   (initially k = n, so binom = 1) */
    seq_t binom = 1;

    /* iterate k from n down to 0 */
    for (idx_t k = n; ; --k) {
        /* compute power = (k+1)^(k-1)  */
        seq_t power;
        if (k <= 1) {
            /* (1)^(-1) and (2)^0 are both 1 */
            power = 1;
        } else {
            /* exponent fits in idx_t, use dedicated routine */
            power = small_pow((int)(k + 1), k - 1);
        }

        /* term = binom * ratio * power */
        seq_t term = binom;
        term *= ratio;
        term *= power;
        sum += term;

        if (k == 0)
            break;

        /* update for next iteration (k-1) */
        /* ratio_{k-1} = ratio_k * k        */
        ratio *= (seq_t)k;

        /* binom_{k-1} = binom_k * (k+2) / (n+1 - k) */
        seq_t num = (seq_t)(k + 2);
        seq_t den = (seq_t)(n + 1 - k);
        binom = binom * num / den;
    }

    return sum;
}
