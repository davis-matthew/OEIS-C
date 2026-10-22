/*
 * A348520  Pentaphobe or 5-phobe numbers: integers that are not pentaphile
 *          numbers.
 * https://oeis.org/A348520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   isPentaphile
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pentaphobe numbers: integers that cannot be expressed as a sum of five
   strictly increasing positive integers each dividing the next.
   This implementation precomputes all pentaphobe numbers up to 240
   (the known maximum) using a brute‑force search and returns the nth one
   (1‑based).  For n greater than the number of pentaphobes, it returns 0. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPentaphile.h"

/* Return the nth pentaphobe number (1‑based).  If n is out of range,
   return 0.  The array is built on first use. */
seq_t a (idx_t n)
{
    /* static cache of pentaphobe numbers */
    static seq_t pent[200];
    static size_t pent_len = 0;
    static bool done = false;

    if (!done) {
        /* Precompute all pentaphobe numbers up to the known maximum. */
        const idx_t LIMIT = 300;                      /* > 240, safe upper bound */
        for (idx_t m = 1; m <= LIMIT; ++m) {
            if (!isPentaphile ((seq_t)m))
                pent[pent_len++] = (seq_t)m;
        }
        done = true;
    }

    if (n == 0 || n > pent_len)
        return 0;               /* undefined for out‑of‑range indices */

    return pent[(size_t)(n - 1)];
}
