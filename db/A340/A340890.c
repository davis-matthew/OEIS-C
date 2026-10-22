/*
 * A340890  a(n) is the number of preference profiles for n men and n women,
 *          where all men prefer the same woman.
 * https://oeis.org/A340890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*((n-1)!^n)*(n!^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Compute factorial(n) using seq_mul, where n >= 0 */
static seq_t factorial(idx_t n)
{
    seq_t r = 1;                      /* seq_t is unsigned, so 1 is fine */
    for (idx_t i = 2; i <= n; ++i) {
        r = seq_mul(r, (seq_t)i);
    }
    return r;
}

/* OEIS A340890:  a(n) = n * ((n-1)!^n) * (n!^n) */
seq_t a(idx_t n)
{
    /* (n-1)! and n! */
    seq_t f1 = factorial(n - 1);          /* (n-1)! */
    seq_t fn = seq_mul(f1, (seq_t)n);     /* n! = n * (n-1)! */

    /* powers */
    seq_t pow_f1 = seq_pow(f1, n);
    seq_t pow_fn = seq_pow(fn, n);

    /* multiply all together: n * pow_f1 * pow_fn */
    seq_t result = (seq_t)n;
    result = seq_mul(result, pow_f1);
    result = seq_mul(result, pow_fn);

    return result;
}
