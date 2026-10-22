/*
 * A382730  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 6.
 * https://oeis.org/A382730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of binomial coefficients in rows 0..n of
 *            Pascal's triangle that are NOT divisible by 6. The value is
 *            computed by constructing each row modulo 6 and counting the
 *            non‑zero residues
 * Range:     u8 n <= 25, u16 n <= 723, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static data structures for memoisation */
    static seq_t *cum = NULL;          /* cumulative totals, cum[i] = a(i) */
    static size_t cum_cap = 0;         /* capacity of cum array (number of elements) */
    static uint8_t *row = NULL;        /* current Pascal row modulo 6 (size cum_cap) */
    static size_t row_cap = 0;         /* capacity of row array */
    static idx_t current_n = 0;         /* highest n that has been processed (initialised to 0) */
    static bool initialized = false;    /* flag to perform one‑time initialisation */

    /* One‑time initialisation: allocate space for a(0) and initialise the first row */
    if (!initialized) {
        cum_cap = 1;
        row_cap = 1;
        cum = (seq_t *)malloc(cum_cap * sizeof(seq_t));
        row = (uint8_t *)malloc(row_cap * sizeof(uint8_t));
        /* row[0] = C(0,0) mod 6 == 1; cum[0] = number of entries in rows 0..0 not divisible by 6 == 1 */
        row[0] = 1;
        cum[0] = (seq_t)1;
        current_n = 0;
        initialized = true;
    }

    /* Grow the storage if we need to process a larger n */
    if (n > current_n) {
        size_t need = (size_t)(n + 1);

        if (cum_cap < need) {
            cum_cap = need;
            cum = (seq_t *)realloc(cum, cum_cap * sizeof(seq_t));
        }

        if (row_cap < need) {
            row_cap = need;
            row = (uint8_t *)realloc(row, row_cap * sizeof(uint8_t));
        }

        /* Extend the Pascal triangle from current_n+1 up to n */
        for (idx_t k = current_n + 1; k <= n; ++k) {
            /* The new row starts with 1 at the last position */
            row[k] = 1;

            /* Update interior elements from right to left so originals are preserved */
            for (idx_t j = k - 1; j > 0; --j) {
                uint8_t left  = row[j - 1];   /* old row[j-1] */
                uint8_t right = row[j];       /* old row[j] */
                uint8_t sum = left + right;   /* sum < 10, so single modulo operation is cheap */
                row[j] = sum % 6;
            }
            /* row[0] remains 1 */

            /* Count entries not divisible by 6 in the new row */
            idx_t count_nonzero = 0;
            for (idx_t j = 0; j <= k; ++j) {
                if (row[j] != 0) {
                    ++count_nonzero;
                }
            }

            /* Update cumulative total */
            cum[k] = cum[k - 1] + (seq_t)count_nonzero;
        }
        current_n = n;
    }

    /* Return the cached answer */
    return cum[(size_t)n];
}
