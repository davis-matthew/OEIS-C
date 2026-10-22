/*
 * A393397  Irregular triangle read by rows: the n-th row gives the list of
 *          (n/d)^d where d is a divisor of n.
 * https://oeis.org/A393397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 116, u32 n <= 271, u64 n <= 611,
 *            u128 n <= 1375, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‐th term of the irregular triangle where
 *  the r‑th row lists (r/d)^d for each divisor d of r in ascending order.
 *  For example:
 *      row 1: 1
 *      row 2: 2, 1
 *      row 3: 3, 1
 *  The function builds the sequence by locating the row containing the
 *  request and then selecting the appropriate divisor. */

/* required helper is supplied by the build:  big_pow(seq_t base, seq_t exp) */
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Count divisors of x by a simple sqrt loop. */
static size_t divisor_count_u64(idx_t x)
{
    size_t count = 0;
    for (idx_t i = 1; i <= x / i; ++i)
    {
        if (x % i == 0)
        {
            ++count;
            if (i != x / i)
                ++count;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    /* Find the row that contains the n‑th element. */
    idx_t row = 1;          /* current row number            */
    idx_t seen = 0;         /* number of terms seen so far   */

    while (seen + divisor_count_u64(row) < n)
    {
        seen += divisor_count_u64(row);
        ++row;
    }

    /* Position inside the found row (0‑based). */
    idx_t pos = n - seen - 1;

    /* Locate the divisor at position 'pos' in ascending order. */
    idx_t divisor = 1;
    for (idx_t d = 1; d <= row; ++d)
    {
        if (row % d == 0)
        {
            if (pos == 0)
            {
                divisor = d;
                break;
            }
            --pos;
        }
    }

    /* Compute (row / divisor)^divisor. */
    seq_t base   = (seq_t)(row / divisor);
    seq_t exp    = (seq_t)divisor;
    return big_pow(base, exp);
}
