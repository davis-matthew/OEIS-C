/*
 * A303115  Total area of the family of rectangles with side lengths p and q
 *          such that n = p + q, p < q, and |q-p| is not semiprime.
 * https://oeis.org/A303115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊(n-1)/2⌋} i*(n-i) * (1 - [Omega(n-2i)=2])
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where Omega(k) is the total number of prime factors of k with multiplicity.  */
#include <stddef.h>
#include <stdbool.h>

/* Utility to compute Omega(n).  */
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t sum = 0;
    idx_t k = (n - 1) / 2;          /* floor((n-1)/2) */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t area = (seq_t)i * (seq_t)(n - i);
        idx_t diff = n - 2 * i;     /* n-2i, always >= 1 for i <= k */
        if (bigomega(diff) == 2)    /* skip if |q-p| is semiprime */
            continue;
        sum += area;
    }
    return sum;
}
