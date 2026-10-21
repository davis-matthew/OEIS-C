/*
 * A296579  Numbers that are not the sum of 3 squares and a nonnegative 9th
 *          power.
 * https://oeis.org/A296579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296579: numbers of the form 16*(8i+7) (0 ≤ i ≤ 152)
   and 64*(8j+7) (0 ≤ j ≤ 37), sorted in increasing order.  The
   sequence contains exactly 191 terms.  The function returns the
   n‑th term (1‑based); for n < 1 or n > 191 it returns 0. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Array that will hold all 191 terms once computed */
    static seq_t data[191];
    /* Flag to ensure one‑time initialization */
    static bool initialized = false;

    if (!initialized)
    {
        size_t ia = 0, ib = 0;   /* counters for the two progressions */
        size_t pos = 0;          /* current position in data[] */

        /* Merge the two arithmetic progressions in ascending order */
        while (ia <= 152 && ib <= 37)
        {
            seq_t val_a = (seq_t)128 * (seq_t)ia + (seq_t)112; /* 16*(8i+7) */
            seq_t val_b = (seq_t)512 * (seq_t)ib + (seq_t)448; /* 64*(8j+7) */

            if (val_a < val_b)
            {
                data[pos++] = val_a;
                ++ia;
            }
            else
            {
                data[pos++] = val_b;
                ++ib;
            }
        }

        /* Flush any remaining terms from the first progression */
        while (ia <= 152)
        {
            data[pos++] = (seq_t)128 * (seq_t)ia + (seq_t)112;
            ++ia;
        }

        /* Flush any remaining terms from the second progression */
        while (ib <= 37)
        {
            data[pos++] = (seq_t)512 * (seq_t)ib + (seq_t)448;
            ++ib;
        }

        initialized = true;   /* initialization complete */
    }

    /* Bounds check: the sequence is defined only for 1 ≤ n ≤ 191 */
    if (n < 1 || n > 191)
        return (seq_t)0;

    return data[(size_t)(n - 1)];
}
