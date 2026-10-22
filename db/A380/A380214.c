/*
 * A380214  Expansion of e.g.f. exp( 1/(1-3*x)^(2/3) - 1 ).
 * https://oeis.org/A380214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380214
   a(n) = Sum_{k=0..n} 2^k * 3^(n-k) * |Stirling1(n,k)| * Bell(k);
   Recurrence used for efficient computation:
   a(n) = 15*(n-2)*a(n-1)
          - 5*(18*n^2-90*n+115)*a(n-2)
          + (270*n^3-2430*n^2+7335*n-7417)*a(n-3)
          - 135*(n-4)*(n-3)*(3*n^2-21*n+37)*a(n-4)
          + 27*(n-5)*(n-4)*(n-3)*(3*n-13)*(3*n-11)*a(n-5);
   Initial values: a(0)=1, a(1)=2, a(2)=14, a(3)=148, a(4)=2076. */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0;

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (seq_cache_size <= n) {
        idx_t old_size = seq_cache_size;
        seq_cache_size = n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cache_size * sizeof(seq_t));
        for (idx_t i = old_size; i <= n; ++i) {
            if (i == 0) {
                seq_cache[0] = (seq_t)1;
            } else if (i == 1) {
                seq_cache[1] = (seq_t)2;
            } else if (i == 2) {
                seq_cache[2] = (seq_t)14;
            } else if (i == 3) {
                seq_cache[3] = (seq_t)148;
            } else if (i == 4) {
                seq_cache[4] = (seq_t)2076;
            } else {
                seq_t nseq = (seq_t)i;

                /* Coefficients (all positive multiplications first) */
                seq_t coeff1 = (seq_t)15 * (nseq - 2);

                seq_t coeff2 = (seq_t)18 * nseq * nseq
                               - (seq_t)90 * nseq
                               + (seq_t)115;
                coeff2 *= (seq_t)5;          /* absolute value */

                seq_t coeff3 = (seq_t)270 * nseq * nseq * nseq
                               - (seq_t)2430 * nseq * nseq
                               + (seq_t)7335 * nseq
                               - (seq_t)7417;

                seq_t coeff4 = (seq_t)135
                               * (nseq - 4)
                               * (nseq - 3)
                               * ((seq_t)3 * nseq * nseq
                                  - (seq_t)21 * nseq
                                  + (seq_t)37);

                seq_t coeff5 = (seq_t)27
                               * (nseq - 5)
                               * (nseq - 4)
                               * (nseq - 3)
                               * ((seq_t)3 * nseq - (seq_t)13)
                               * ((seq_t)3 * nseq - (seq_t)11);

                /* Terms with appropriate signs */
                seq_t term1 = coeff1 * seq_cache[i - 1];
                seq_t term2 = coeff2 * seq_cache[i - 2];
                seq_t term3 = coeff3 * seq_cache[i - 3];
                seq_t term4 = coeff4 * seq_cache[i - 4];
                seq_t term5 = coeff5 * seq_cache[i - 5];

                /* a(i) = term1 - term2 + term3 - term4 + term5 */
                seq_t result = term1;
                result = result - term2;
                result = result + term3;
                result = result - term4;
                result = result + term5;

                seq_cache[i] = result;
            }
        }
    }
    return seq_cache[n];
}
