/*
 * A261611  Expansion of Product_{k>=0} (1 + x^(4*k+1))/(1 - x^(4*k+1)).
 * https://oeis.org/A261611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in ∏_{k≥0} (1 + x^(4k+1)) / (1 –
 *            x^(4k+1))
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include only standard headers that are guaranteed to be
    available on all supported compilers.  */
#include <stdlib.h>

/*  Cached values of the sequence.  */
static seq_t *dp = NULL;          /* array holding coefficients up to the largest n computed  */
static size_t max_n_computed = 0;

/*  Return the nth term of the sequence.  */
seq_t a(idx_t n)
{
    /*  Return a cached value if possible.  */
    if (dp != NULL && n <= (idx_t)max_n_computed)
        return dp[n];

    /*  Need to compute up to a new maximum index.  */
    size_t new_max = n;
    /*  Allocate temporary working arrays.  */
    seq_t *dp_old = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
    seq_t *dp_new = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
    if (dp_old == NULL || dp_new == NULL) {
        /*  In a real system we would handle allocation failures.  */
        exit(EXIT_FAILURE);
    }
    dp_old[0] = 1;           /*  a(0) = 1  */

    /*  Process each part size m = 1, 5, 9, … ≤ new_max.  */
    for (idx_t m = 1; m <= new_max; m += 4) {
        /*  Copy the current state to dp_new.  */
        for (size_t i = 0; i <= new_max; ++i)
            dp_new[i] = dp_old[i];

        /*  For each residue class modulo m, accumulate contributions
            efficiently with a running sum.  */
        for (idx_t offset = 0; offset < m; ++offset) {
            seq_t running = 0;
            for (idx_t i = offset + m; i <= new_max; i += m) {
                running += dp_old[i - m];                      /*  sum of dp_old[i-m] + dp_old[i-2m] + …  */
                dp_new[i] = dp_old[i] + 2 * running;           /*  a_new[i] = a_old[i] + 2 * running  */
            }
        }
        /*  Prepare for the next part size.  */
        seq_t *tmp = dp_old;
        dp_old = dp_new;
        dp_new = tmp;
    }

    /*  Free the previous global cache (if any).  */
    if (dp != NULL)
        free(dp);
    /*  Store the new cache – dp_old contains the final data.  */
    dp = dp_old;
    /*  dp_new now holds the old temporary array; free it.  */
    free(dp_new);
    max_n_computed = new_max;

    return dp[n];
}
