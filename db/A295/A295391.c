/*
 * A295391  a(1) = 2; a(n+1) = 1 + (a(n)^2 - a(n))*(1+1/n) for n >= 1.
 * https://oeis.org/A295391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n+1) = 1 + (a(n)^2 - a(n))*(1+1/n) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1; return 0 for safety. */
        return (seq_t)0;
    }

    seq_t a_val = (seq_t)2;          /* a(1) = 2 */
    if (n == 1) {
        return a_val;
    }

    /* Iteratively compute terms up to n. */
    for (idx_t k = 1; k < n; ++k) {  /* k corresponds to a(k) */
        seq_t temp   = a_val * a_val - a_val;          /* a(k)^2 - a(k) */
        seq_t factor = (seq_t)k + 1;                   /* n + 1  (since k = n) */
        seq_t mult   = temp * factor;                 /* (a(k)^2 - a(k)) * (n+1) */
        a_val = (seq_t)1 + mult / (seq_t)k;           /* next term */
    }

    return a_val;
}
