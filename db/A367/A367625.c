/*
 * A367625  (A036991(n)-1)/2.
 * https://oeis.org/A367625
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A036991(n)-1)/2; computed by search over odd k whose
 *            binary expansion, read from the low bit up, never has more 0's
 *            than 1's
 * Range:     u8 n <= 106, u16 n <= 13496, u32 n <= 13496, u64 n <= 13496,
 *            u128 n <= 13496, big any n
 * Validated: standard, pass, 94 terms through n = 8931 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool A036991_valid(seq_t x)
{
    seq_t bal = 0;
    while (x > 0) {
        if ((x & 1) != 0)
            bal++;
        else {
            if (bal == 0)
                return false;
            bal--;
        }
        x >>= 1;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* A036991(1) = 0, A036991(2) = 1, then the remaining valid numbers are odd */
    idx_t seen = 1;          /* the term 0 */
    seq_t k = 1;
    while (1) {
        if (A036991_valid(k)) {
            seen++;
            if (seen == n)
                return (k - 1) / 2;
        }
        k += 2;
    }
}
