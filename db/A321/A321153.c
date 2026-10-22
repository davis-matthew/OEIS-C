/*
 * A321153  Possible total numbers of pips when rolling two dice in
 *          backgammon.
 * https://oeis.org/A321153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate all dice pairs (x, y) ∈ [1..6]², 
   compute the total as (x == y ? 4*x : x + y),
   collect all distinct totals, sort them in ascending order,
   and return the (n‑1)th element (a(1) is the first value). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static table of all possible totals; computed only once */
    static seq_t totals[13];
    static bool initialized = false;
    if (!initialized) {
        /* generate all totals */
        idx_t count = 0;
        for (idx_t x = 1; x <= 6; ++x) {
            for (idx_t y = 1; y <= 6; ++y) {
                seq_t t = (x == y) ? (seq_t)(4 * x) : (seq_t)(x + y);

                /* check if t is already present */
                bool present = false;
                for (idx_t i = 0; i < count; ++i) {
                    if (totals[i] == t) {
                        present = true;
                        break;
                    }
                }
                if (!present) {
                    totals[count++] = t;
                }
            }
        }

        /* sort the collected totals */
        for (idx_t i = 0; i < count - 1; ++i) {
            for (idx_t j = i + 1; j < count; ++j) {
                if (totals[j] < totals[i]) {
                    seq_t tmp = totals[i];
                    totals[i] = totals[j];
                    totals[j] = tmp;
                }
            }
        }

        initialized = true;
    }

    /* return the n‑th term (1‑based) if it exists */
    if (n - 1 < 13) {
        return totals[n - 1];
    }
    /* for n beyond the defined terms, return 0 (undefined) */
    return 0;
}
