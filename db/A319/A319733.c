/*
 * A319733  a(n) is the sum of the digits of all positive integers k <= 2^n.
 * https://oeis.org/A319733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of digits of all positive integers k ≤ 2^n
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   digits_in_seq, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of digits of all numbers from 0 to N (inclusive) is computed
   using the standard positional-digit formula.  */
   
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow2.h"    /* pow2(idx_t n) -> seq_t  */
#include "../utils/seq_mul.h"/* seq_mul(seq_t a, seq_t b) -> seq_t */
#include "../utils/digits_in_seq.h"

/* Main sequence generator.  External linkage as required. */
seq_t a(idx_t n) {
    /* 1. Compute N = 2^n. */
    seq_t N = pow2(n);

    /* 2. Determine maximum number of decimal digits of N. */
    idx_t dmax = digits_in_seq(N);

    /* 3. Allocate and pre‑compute powers of 10 and sum10[d] for d=0..dmax. */
    /*    pow10[d]   = 10^d
        sum10[d]   = sum of the digits of all numbers from 0 to 10^d - 1
                     = 45 * d * 10^(d-1)  (for d>0, sum10[0]=0). */
    seq_t *pow10  = (seq_t *)malloc((dmax + 1) * sizeof(seq_t));
    seq_t *sum10  = (seq_t *)malloc((dmax + 1) * sizeof(seq_t));
    if (!pow10 || !sum10) { /* minimal error handling: return 0 on allocation fail */
        free(pow10);
        free(sum10);
        return (seq_t)0;
    }
    pow10[0] = (seq_t)1;
    sum10[0] = (seq_t)0;
    for (idx_t i = 1; i <= dmax; ++i) {
        pow10[i] = seq_mul(pow10[i-1], (seq_t)10);
        /* 45*i fits in idx_t; cast to seq_t before multiplication. */
        sum10[i] = seq_mul(pow10[i-1], (seq_t)(45 * i));
    }

    /* 4. Compute the digit sum from 0 to N using the formula. */
    seq_t total = (seq_t)0;
    seq_t cur   = N;
    while (cur > (seq_t)9) {
        idx_t  digs = digits_in_seq(cur);
        idx_t  d    = digs - 1;
        seq_t  p    = pow10[d];
        seq_t  msd  = cur / p;
        seq_t  rem  = cur % p;

        /* Term 1: msd * sum10[d]  -- sum of lower d digits for all full cycles. */
        seq_t term1 = seq_mul(msd, sum10[d]);

        /* Term 2: (msd*(msd-1)/2) * p  -- contribution of the most significant digit in
           all full cycles of the current position. */
        seq_t temp2 = seq_mul(msd, msd - (seq_t)1);
        temp2 /= (seq_t)2;
        seq_t term2 = seq_mul(temp2, p);

        /* Term 3: msd * (rem + 1)  -- contribution of the most significant digit
           for the incomplete cycle (numbers that start with msd). */
        seq_t term3 = seq_mul(msd, rem + (seq_t)1);

        total += term1;
        total += term2;
        total += term3;

        cur = rem;   /* recurse on the remaining lower digits */
    }

    /* 5. Base case for the last (single‑digit) block: numbers 0..cur. */
    total += cur * (cur + (seq_t)1) / (seq_t)2;

    free(pow10);
    free(sum10);
    return total;
}
