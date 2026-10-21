/*
 * A249345  The exponent of the highest power of 5 dividing the product of the
 *          elements on the n-th row of Pascal's triangle.
 * https://oeis.org/A249345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (2*i - n - 1) * v5(i), where v5(i) is the
 *            exponent of 5 in i
 * Range:     u8 n <= 124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   v5_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A249345 – exponent of the highest power of 5 dividing
    the product of the elements on the n‑th row of Pascal's triangle. */
/*  All intermediate arithmetic uses signed long long to avoid
    unsigned wrap‑around on negative coefficients.  Function assumes
    idx_t and seq_t are defined by the build environment.  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/v5_of_uint64.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Avoid signed overflow by using signed long long.  */
    long long sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned int vi = v5_of_uint64(i);
        /* Compute coefficient (2*i - n - 1) safely in signed long long. */
        long long coeff = (long long)i * 2LL - (long long)n - 1LL;
        sum += coeff * (long long)vi;
    }
    /* Final result is always non‑negative; cast to seq_t. */
    return (seq_t)sum;
}
