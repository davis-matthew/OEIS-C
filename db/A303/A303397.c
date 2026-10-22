/*
 * A303397  Expansion of Product_{k>=1} (1 - 2*x^k)/(1 + 2*x^k).
 * https://oeis.org/A303397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A303397:  a(n) is coefficient of ∏_{k≥1} (1-2x^k)/(1+2x^k) */
/*  a(0)=1,a(1)=-4,a(2)=4,a(3)=-4,a(4)=20,...  */
/*  The implementation builds the numerator and denominator series
    up to n, then solves a * D = N for the coefficient series a.   */
/*  All arithmetic is done using seq_t and satisfies the constraints
    described in the problem statement.                         */

#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /*  Convert idx_t to a size_t for array indexing  */
    size_t N = (size_t)n;
    size_t sz = N + 1;

    /* Allocate coefficient arrays  */
    seq_t *num = seq_alloc_new_array(sz);
    seq_t *den = seq_alloc_new_array(sz);
    seq_t *ans = seq_alloc_new_array(sz);

    /* Initialise arrays  */
    num[0] = (seq_t)1;
    den[0] = (seq_t)1;
    for (size_t i = 1; i < sz; ++i) {
        num[i] = (seq_t)0;
        den[i] = (seq_t)0;
    }

    /* Build numerator N(x) = ∏(1-2x^k) and denominator D(x) = ∏(1+2x^k)  */
    for (size_t k = 1; k <= N; ++k) {
        for (size_t j = N; j >= k; --j) {
            num[j] += num[j - k] * (seq_t)(-2);
            den[j] += den[j - k] * (seq_t)( 2);
        }
    }

    /* Solve a * D = N  for a  (D[0]=1)  */
    ans[0] = num[0];  /* a(0)=1  */
    for (size_t m = 1; m <= N; ++m) {
        seq_t s = (seq_t)0;
        for (size_t j = 0; j < m; ++j) {
            s += ans[j] * den[m - j];
        }
        ans[m] = num[m] - s;
    }

    seq_t result = ans[N];

    /* Free temporary arrays  */
    seq_free_array(num);
    seq_free_array(den);
    seq_free_array(ans);

    return result;
}
