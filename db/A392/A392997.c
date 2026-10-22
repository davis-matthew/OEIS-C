/*
 * A392997  Expansion of e.g.f. (1/x) * Series_Reversion( x * (exp(-x) - x^2)
 *          ).
 * https://oeis.org/A392997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n+k+1)^(n-2*k-1) *
 *            binomial(n+k+1,k)/(n-2*k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    /* Cache for factorials up to n */
    static seq_t *fact = NULL;
    static size_t fact_cap = 0;

    if ((size_t)n + 1 > fact_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */

        if (fact_cap == 0) {
            /* initial factorial */
            tmp[0] = (seq_t)1;
            fact_cap = 1;
        } else {
            fact = tmp;
        }

        /* fill missing factorials */
        for (size_t i = fact_cap; i < new_cap; ++i)
            tmp[i] = tmp[i-1] * (seq_t)i;
        fact = tmp;
        fact_cap = new_cap;
    }

    seq_t sum = (seq_t)0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t n2k = n - 2 * k;          /* n - 2k   (>=0) */
        seq_t ratio = fact[n] / fact[n2k];  /* n! / (n-2k)! */
        idx_t nk1 = n + k + 1;           /* n + k + 1 */
        seq_t base = (seq_t)nk1;

        /* binomial coefficient C(n+k+1, k) */
        seq_t binom = binomial((idx_t)(n + k + 1), k);

        seq_t term;
        if (n2k == 0) {
            /* exponent is -1: divide by base */
            term = (ratio * binom) / base;
        } else {
            /* exponent is (n-2k-1) >= 0 */
            seq_t pow_val = big_pow(base, (seq_t)(n2k - 1));
            term = ratio * binom * pow_val;
        }

        sum += term;
    }

    return sum;
}
