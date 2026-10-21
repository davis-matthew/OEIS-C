/*
 * A280086  Partial products of A206032 (Product_{d|n} sigma(d)).
 * https://oeis.org/A280086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d=1..n} sigma(d)^{floor(n/d)}
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* allocate array to hold sigma values for 1..n */
    seq_t *sigma = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!sigma) return (seq_t)0;          /* safe guard – should not happen */

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) sigma[i] = 0;

    /* compute sigma(d) by summing divisors for each multiple */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t add = (seq_t)d;
        for (idx_t m = d; m <= n; m += d) {
            sigma[m] += add;
        }
    }

    /* compute the product ∏ sigma(d)^{floor(n/d)} */
    seq_t result = (seq_t)1;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t exp   = (seq_t)(n / d);         /* floor division */
        seq_t pow   = big_pow(sigma[d], exp); /* sigma(d)^exp */
        result = seq_mul(result, pow);
    }

    free(sigma);
    return result;
}
