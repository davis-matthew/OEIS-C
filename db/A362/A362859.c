/*
 * A362859  Expansion of e.g.f. exp(-x) / (1 + LambertW(-2*x)).
 * https://oeis.org/A362859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n * Sum_{k=0..n} (-2*k)^k * binomial(n,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */
#include "../utils/big_pow.h"    /* big_pow(seq_t base, seq_t exp) */

/* Sequence calculation */
seq_t a(idx_t n)
{
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin = binomial(n, k);

        seq_t powk;
        if (k == 0) {
            powk = 1;                       /* 0^0 is defined as 1 in this formula */
        } else {
            seq_t base = 2 * ((seq_t)k);    /* base = 2*k   */
            powk = big_pow(base, k);        /* (2*k)^k       */
        }

        seq_t term = bin * powk;            /* current summand */

        /* sign = (-1)^{n+k} */
        if (((n + k) & 1) == 0) {
            sum_pos += term;               /* even parity -> positive contribution */
        } else {
            sum_neg += term;               /* odd parity -> negative contribution  */
        }
    }

    return sum_pos - sum_neg;              /* final a(n)  */
}
