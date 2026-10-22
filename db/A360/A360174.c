/*
 * A360174  Triangle read by rows. T(n, k) = (k + 1) * abs(Stirling1(n, k)).
 * https://oeis.org/A360174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+1) * |Stirling1(r, k)| where r =
 *            floor((sqrt(1+8*n)-1)/2) k = n - r*(r+1)/2
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Commercial and scientific work is protected under
    the same licenses as the OEIS entry.   */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* 1. Determine the triangle row r such that
         r*(r+1)/2 <= n < (r+1)*(r+2)/2                                 */
    double d = sqrt((double)(8.0 * (double)n + 1.0));
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct possible rounding errors of the double sqrt */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row */
    unsigned __int128 base = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)base;

    /* 2. Compute unsigned Stirling numbers of the first kind
         for the row r                                      */
    seq_t *c = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!c)
        return (seq_t)0;       /* memory allocation failed */

    for (idx_t i = 0; i <= r; ++i)
        c[i] = 0;
    c[0] = 1;                /* s(0,0) = 1 */

    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            seq_t new_val = c[j - 1] + (seq_t)(i - 1) * c[j];
            c[j] = new_val;
        }
        c[0] = 0;
    }

    seq_t value   = c[k];
    seq_t result  = (seq_t)(k + 1) * value;

    free(c);
    return result;
}
