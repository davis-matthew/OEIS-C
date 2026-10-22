/*
 * A380851  Riordan array ((1-x)^(m-1), x/(1-x)) with factor r^(2*n) on row n,
 *          for m = 3/2, r = 2.
 * https://oeis.org/A380851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth entry of the Riordan array
 * ((1‑x)^(3/2‑1), x/(1‑x)) multiplied by r^(2n)=4^n,
 * enumerated row by row.
 *
 * Let r = ⌊(√(8n+1)−1)/2⌋ and c = n − r(r+1)/2   (0‑based indexes)
 * Then i = r − c.  The value is:
 *
 *      a(n) = 2^(2r−i) * (−1)^i *  ∏_{j=0}^{i‑1} (1 – 2c – 2j) / i!
 *
 * where the product over an empty set (i==0) is 1.
 *
 * All arithmetic is done with seq_t, which may be an arbitrary precision
 * integer type; no floating‑point arithmetic except for the square root
 * used to locate r.
 */

#include <math.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* Find row r and column c from the index n */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    /* Correct possible rounding errors */
    while (r * (r + 1) / 2 > n) r--;
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    idx_t c = n - r * (r + 1) / 2;          /* column in that row */
    idx_t i = r - c;                        /* distance from diagonal */

    /* Compute the product ∏(1 – 2c – 2j) */
    seq_t prod = 1;
    for (idx_t j = 0; j < i; ++j) {
        seq_t term = (seq_t)1 - (seq_t)2 * (seq_t)c - (seq_t)2 * (seq_t)j;
        prod *= term;
    }

    /* Compute 2^(2r – i) */
    idx_t exp = 2 * r - i;
    seq_t pow2 = seq_pow((seq_t)2, exp);

    /* Assemble numerator: prod * 2^(2r-i) with sign (−1)^i */
    seq_t num = prod * pow2;
    if (i & 1) {
        num = -num;                        /* apply (−1)^i */
    }

    /* Compute denominator i! */
    seq_t fact = 1;
    for (idx_t k = 1; k <= i; ++k) {
        fact *= (seq_t)k;
    }

    /* Final result */
    return num / fact;
}
