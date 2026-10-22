/*
 * A354863  a(n) = n! * Sum_{d|n} (n/d) / d!.
 * https://oeis.org/A354863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{d|n} (n/d) / d! (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   factorial_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factorial_idx.h"

/* Compute the n-th term of sequence A354863.
   n is 1‑based; behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined below offset */
        return (seq_t)0;
    }

    seq_t fact_n = factorial_idx(n);
    seq_t result = (seq_t)0;
    idx_t d;

    /* iterate through each divisor d of n, using the usual pairing trick */
    for (d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        /* divisor d */
        seq_t fact_d = factorial_idx(d);      /* d!   */
        seq_t term   = fact_n / fact_d;       /* n! / d!  */
        term *= (seq_t)(n / d);               /* * (n/d)  */
        result += term;

        /* paired divisor n/d if distinct from d */
        idx_t d2 = n / d;
        if (d2 != d) {
            seq_t fact_d2 = factorial_idx(d2);
            seq_t term2   = fact_n / fact_d2;
            term2 *= (seq_t)(n / d2);
            result += term2;
        }
    }

    return result;
}
