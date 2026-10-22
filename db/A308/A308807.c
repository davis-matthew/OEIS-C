/*
 * A308807  a(n) = 4*5^(n-1) + n.
 * https://oeis.org/A308807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*5^(n-1) + n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For safety we handle n==0. */
    if (n == 0) return (seq_t)0;

    /* Compute 5^(n-1) using the provided big‑integer power helper. */
    seq_t pow5 = seq_pow((seq_t)5, n - 1);

    /* a(n) = 4*5^(n-1) + n.  Convert n to seq_t for exact arithmetic. */
    return (seq_t)4 * pow5 + (seq_t)n;
}
