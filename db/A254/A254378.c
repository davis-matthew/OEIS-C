/*
 * A254378  Run lengths of A228495 (Characteristic function of the odd odious
 *          numbers).
 * https://oeis.org/A254378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the n‑th run in the binary sequence A228495
 *            (the odd‑odious characteristic sequence). Computed by iterating
 *            over indices and counting consecutive equal values
 * Range:     u8 n <= 9175, u16 n <= 9175, u32 n <= 9175, u64 n <= 9175,
 *            u128 n <= 9175, big any n
 * Helpers:   is_odd_odious
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_odd_odious.h"

seq_t a(idx_t n)
{
    /* n is a 1‑based index into the run‑length sequence. */
    idx_t i = 0;
    bool cur = is_odd_odious(i);
    idx_t len = 1;
    idx_t run_index = 0;

    for (;;)
    {
        idx_t next_i = i + 1;
        bool next = is_odd_odious(next_i);

        if (next == cur)
            len++;
        else {
            run_index++;
            if (run_index == n)
                return (seq_t)len;   /* finished the nth run */
            cur = next;
            len = 1;
        }
        i = next_i;
    }
}
