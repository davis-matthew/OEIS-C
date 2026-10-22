/*
 * A368293  Expansion of e.g.f. exp(2*x) / (1 - x*exp(3*x)).
 * https://oeis.org/A368293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} ((3*(n-k)+2)^k)/(k!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Allocate array r[0..n] where r[k] = n! / k!   (computed backwards) */
    seq_t *r = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!r) return 0; /* allocation failure, return 0 as a fallback */

    /* r[n] = 1  (since n!/n! = 1) */
    r[n] = (seq_t)1;

    /* Compute r[k] for k = n-1 down to 0 */
    for (idx_t k = n; k > 0; --k) {
        r[k - 1] = r[k] * (seq_t)k; /* r[k-1] = r[k] * k */
    }

    /* Sum the terms: term_k = r[k] * ((3*(n-k)+2)^k) */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)3 * (seq_t)(n - k) + (seq_t)2;
        seq_t power = big_pow(base, (seq_t)k);      /* power = base^k */
        seq_t term  = r[k] * power;                 /* term = r[k] * power */
        sum += term;
    }

    free(r);
    return sum;
}
