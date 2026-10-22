/*
 * A351409  a(n) = n*(n!)^(2*n-2).
 * https://oeis.org/A351409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n!)^(2*n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_pow_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow_exp.h"

/* Compute n! for idx_t n */
static seq_t factorial(idx_t n) {
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        f *= (seq_t)i;
    }
    return f;
}

/* OEIS sequence A351409 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* below offset, return 0 */
    seq_t fact = factorial(n);
    idx_t exp = 2*n - 2;
    seq_t power = seq_pow_exp(fact, exp);
    return (seq_t)n * power;
}
