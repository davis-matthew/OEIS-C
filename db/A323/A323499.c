/*
 * A323499  Number of minimum dominating sets in the n-triangular (Johnson)
 *          graph.
 * https://oeis.org/A323499
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!! for n odd; a(n) = (n-1)!! * (1 + n * (n/2 - 1)) for n
 *            even
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 31, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the odd double‑factorial  n!!  for odd  n. */
static seq_t odd_double_factorial(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; i += 2) {
        res *= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0; /* value below the offset is irrelevant */
    }
    if (n & 1u) {   /* n is odd */
        return odd_double_factorial(n);
    } else {        /* n is even */
        seq_t oddFact = odd_double_factorial(n - 1);
        seq_t part = (seq_t)n * (((seq_t)(n / 2)) - 1);
        part = 1 + part;
        return oddFact * part;
    }
}
