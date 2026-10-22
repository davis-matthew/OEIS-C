/*
 * A379212  Expansion of Sum_{n >= 0} q^(n*(n+1)) * Product_{k >= 2*n+2} 1 -
 *          q^k.
 * https://oeis.org/A379212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of q^n in * ∏_{k≥0} (1 - q^(20k+3)) (1 -
 *            q^(20k+4)) (1 - q^(20k+7)) * (1 - q^(10k+10)) (1 - q^(20k+13))
 *            (1 - q^(20k+16)) (1 - q^(20k+17))
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Generate all factors (1 - q^m) for m ≤ n and multiply them
 * via a simple coefficient DP.  The DP array indexes are 0 … n. */
seq_t a(idx_t n)
{
    /* Work only with the required range up to n. */
    size_t N = (size_t)n;

    /* Allocate an array of seq_t values, one for each exponent. */
    seq_t *dp = seq_alloc_new_array(N + 1);
    if (!dp)
        return 0;   /* allocation failure → 0 */

    /* Initialise the array to zero and set the constant term. */
    for (size_t i = 0; i <= N; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* coefficient of q^0 */

    /* Apply the factors with period 20: residues 3,4,7,13,16,17. */
    static const int residues20[] = {3,4,7,13,16,17};
    for (size_t ridx = 0; ridx < sizeof(residues20)/sizeof(residues20[0]); ++ridx) {
        size_t r = (size_t)residues20[ridx];
        for (size_t m = r; m <= N; m += 20) {
            for (size_t d = N; d >= m; --d)
                dp[d] -= dp[d - m];
        }
    }

    /* Apply the factors with period 10: residue 10. */
    size_t r = 10;
    for (size_t m = r; m <= N; m += 10) {
        for (size_t d = N; d >= m; --d)
            dp[d] -= dp[d - m];
    }

    seq_t ans = dp[N];
    seq_free_array(dp);
    return ans;
}
