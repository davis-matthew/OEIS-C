/*
 * A328732  Irregular table read by rows; for any n >= 0, the n-th row
 *          contains the numbers of the form u - v with u + v = n and u AND v
 *          = 0 (where AND denotes the bitwise AND operator), in ascending
 *          order.
 * https://oeis.org/A328732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, tabf
 * Formula:   not stated
 * Range:     i8 n <= 2187, i16 n <= 6562, i32 n <= 6562, i64 n <= 6562,
 *            i128 n <= 6562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by enumerating all subsets of the 1‑bits of the row number.
   The n‑th row consists of the values 2*u − row where u ranges over all
   subsets of the set bits of row.  The sequence is the table read row by
   row, concatenated.  This implementation works for any seq_t type
   (including arbitrary‑precision) and for all n ≥ 0. */
#include <stdint.h>   /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    idx_t cumulative = 0;               /* number of terms produced so far   */
    for (idx_t row = 0;; ++row) {
        unsigned bits = __builtin_popcountll(row);
        idx_t term_count = (bits == 0) ? 1 : ((idx_t)1 << bits);

        if (cumulative + term_count > n) {
            /* a(n) lies in this row */
            idx_t offset = n - cumulative;   /* position within the row (0‑based) */

            /* collect the positions of the 1‑bits of 'row' in increasing order */
            unsigned bitpos[64];
            unsigned bc = 0;
            idx_t tmp = row;
            idx_t bit_index = 0;
            while (tmp) {
                if (tmp & 1)
                    bitpos[bc++] = (unsigned)bit_index;
                ++bit_index;
                tmp >>= 1;
            }

            /* compute u for the given offset (mask) */
            idx_t u = 0;
            for (unsigned i = 0; i < bc; ++i) {
                if (offset & ((idx_t)1 << i))
                    u += ((idx_t)1 << bitpos[i]);
            }

            /* final value: 2*u - row */
            seq_t val = (seq_t)((seq_t)u * 2 - (seq_t)row);
            return val;
        }

        cumulative += term_count;
    }
}
