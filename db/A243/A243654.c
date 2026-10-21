/*
 * A243654  Nonnegative numbers represented by the indefinite quadratic form
 *          3x^2+5xy-3y^2, of discriminant 61.
 * https://oeis.org/A243654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: This function computes the n‑th nonnegative integer
   represented by the indefinite quadratic form 3x²+5xy-3y²
   (discriminant 61).  It searches over all integer pairs (x,y)
   within a bounded range, gathers all distinct nonnegative values
   of the form, and selects the n‑th smallest value.  The search
   limit is doubled until the n‑th term is found. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* offset 1 */

    idx_t limit = n * 10;   /* initial bound */

    while (1)
    {
        /* Boolean array for values up to 'limit' */
        uint8_t *present = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
        if (!present)
            return (seq_t)0; /* out of memory */

        /* Bound for |x| and |y| */
        int64_t bound = (int64_t)(sqrt((double)limit) * 2.0) + 10;

        /* Enumerate all pairs (x,y) */
#if defined(__SIZEOF_INT128__)
        for (int64_t x = -bound; x <= bound; ++x)
        {
            for (int64_t y = -bound; y <= bound; ++y)
            {
                __int128 xx = x;
                __int128 yy = y;
                __int128 q  = 3 * xx * xx + 5 * xx * yy - 3 * yy * yy;
                if (q < 0 || q > ( __int128)limit)
                    continue;
                present[(size_t)q] = 1;
            }
        }
#else
        for (int64_t x = -bound; x <= bound; ++x)
        {
            for (int64_t y = -bound; y <= bound; ++y)
            {
                long long xx = x;
                long long yy = y;
                long long q  = 3 * xx * xx + 5 * xx * yy - 3 * yy * yy;
                if (q < 0 || q > (long long)limit)
                    continue;
                present[(size_t)q] = 1;
            }
        }
#endif

        /* Find the n‑th representable value */
        idx_t count = 0;
        seq_t result = 0;

        for (size_t val = 0; val <= (size_t)limit; ++val)
        {
            if (present[val])
            {
                ++count;
                if (count == n)
                {
                    result = (seq_t)val;
                    free(present);
                    return result;
                }
            }
        }

        free(present);
        /* If not enough values found, increase the search limit */
        limit = limit * 2 + 1;
    }
}
