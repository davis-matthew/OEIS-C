/*
 * A329817  a(n) is the maximum number of digits that must be checked to
 *          obtain all Armstrong numbers in base n.
 * https://oeis.org/A329817
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = max{ d | n^(d-1) <= d*(n-1)^d }
 * Range:     u8 n <= 29, u16 n <= 3409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9540 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

static bool inequality_holds(idx_t n, idx_t d)
{
    /* Compare n^(d-1) and d*(n-1)^d using logarithms
       to avoid overflow.  All computations are in double
       because they are not part of the seq_t value. */
    double ln_n   = log((double)n);
    double ln_n1  = log((double)(n - 1));
    double ln_d   = log((double)d);

    double left  = (double)(d - 1) * ln_n;
    double right = ln_d + (double)d * ln_n1;

    /* left <= right -> inequality holds */
    return left <= right;
}

seq_t a(idx_t n)
{
    if (n < 2)   /* offset is 2 – value below that is irrelevant */
        return (seq_t)0;

    /* Find an upper bound for the answer by doubling until inequality fails. */
    idx_t high = 1;
    while (inequality_holds(n, high)) {
        /* Avoid infinite loop for extremely large n (not expected for n <= 10000). */
        if (high > (idx_t)1ULL << 60) {   /* safety cap */
            break;
        }
        high <<= 1;
    }

    /* binary search for the maximum d satisfying the inequality */
    idx_t low = high >> 1;   /* previous high that held */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (inequality_holds(n, mid))
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
