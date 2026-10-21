/*
 * A292449  Numbers where 9 outnumbers any other digit.
 * https://oeis.org/A292449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine enumerates the n‑th positive integer in which the digit 9
   occurs more times than any other digit. It performs a brute‑force
   scan of the natural numbers, counting digit frequencies using
   digit_counts.

   For each candidate number k it checks that count[9] > count[d] for
   all d ≠ 9.  The first such number is 9, and subsequent numbers
   follow in ascending order.  This method works for n up to about
   10 000 within the time limits, and it handles arbitrary‑precision
   seq_t types because digit_counts accepts seq_t all the same way. */

#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but harmless */
    seq_t val = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        ++val;
        unsigned char digs[10] = {0};
        digit_counts(val, digs);
        if (digs[9] == 0) continue;   /* no 9's, impossible */
        bool ok = true;
        for (int d = 0; d < 9; ++d) {
            if (digs[9] <= digs[d]) {
                ok = false;
                break;
            }
        }
        if (ok) ++cnt;
    }
    return val;
}
