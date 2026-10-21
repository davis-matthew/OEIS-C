/*
 * A279931  The maximum length of a sequence of consecutive composite integers
 *          in the closed interval from n^2 to (n+1)^2.
 * https://oeis.org/A279931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: longest consecutive composite run in [n^2,(n+1)^2] */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined for n < 2 */

    seq_t max_run = 0;
    seq_t cur_run = 0;

    /* use idx_t for the interval endpoints to avoid overflow */
    idx_t sq_n   = (idx_t)n * (idx_t)n;
    idx_t sq_np1 = (idx_t)(n + 1) * (idx_t)(n + 1);

    for (idx_t x = sq_n; x <= sq_np1; ++x)
    {
        if (!is_prime(x))          /* composite */
        {
            ++cur_run;
            if (cur_run > max_run) max_run = cur_run;
        }
        else                       /* prime breaks the run */
        {
            cur_run = 0;
        }
    }

    return max_run;
}
