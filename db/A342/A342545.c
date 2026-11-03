/*
 * A342545  a(n)^2 is the least square that has exactly n 0's in base n.
 * https://oeis.org/A342545
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is defined as a(n)^2 is the least square that has exactly n
 *            0's in base n
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 14, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 694 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Count the number of zero digits of x in base 'base'.
   'x' is a positive seq_t value.                                   */
static unsigned
count_zeros(seq_t x, idx_t base)
{
    unsigned cnt = 0;
    while (x > 0) {
        if ((x % base) == 0)
            ++cnt;
        x /= base;
    }
    return cnt;
}
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2. */
    if (n % 2 == 0) {
        /* Even n: a(n) = n^(n/2) */
        return ipow((seq_t)n, (seq_t)(n / 2));
    } else {
        /* Odd n: a(n) = n^((n-3)/2) * s, where s^2 has exactly 3 zeros in base n. */
        idx_t exp = (n - 3) / 2;
        seq_t factor = ipow((seq_t)n, (seq_t)exp);

        /* Search for the minimal s such that s^2 has exactly 3 zeros (base n).   */
        idx_t s = 1;
        while (true) {
            seq_t sq = (seq_t)s * (seq_t)s;
            if (count_zeros(sq, n) == 3)
                break;
            ++s;
        }
        return factor * (seq_t)s;
    }
}
