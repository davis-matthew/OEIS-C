/*
 * A367627  a(n) = log_2(A367626(n)).
 * https://oeis.org/A367627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = log_2(A036991(n+1) - A036991(n)); computed by search over
 *            valid numbers
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9672 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool A367627_valid(seq_t x)
{
    seq_t bal = 0;
    while (x > 0) {
        if (x & 1)
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
    idx_t seen = 0;
    seq_t k = 0;
    seq_t x = 0;
    while (1) {
        if (A367627_valid(k)) {
            seen++;
            if (seen == n) {
                x = k;
                break;
            }
        }
        k++;
    }
    seq_t y = x + 1;
    while (!A367627_valid(y))
        y++;
    seq_t d = y - x;
    seq_t r = 0;
    while (d > 1) {
        d >>= 1;
        r++;
    }
    return r;
}
