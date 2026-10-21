/*
 * A251699  a(n) = (7*n+1) * (8*n+1)^(n-2) * 9^n.
 * https://oeis.org/A251699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251699: a(n) = (7*n+1) * (8*n+1)^(n-2) * 9^n   (offset 0) */

#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the first two indices explicitly (negative exponent otherwise). */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t term = (seq_t)(7 * n + 1);
    seq_t pow8   = seq_pow((seq_t)(8 * n + 1), n - 2); /* (8*n+1)^(n-2) */
    seq_t pow9   = seq_pow((seq_t)9, n);                /* 9^n */

    term = seq_mul(term, pow8);
    term = seq_mul(term, pow9);

    return term;
}
