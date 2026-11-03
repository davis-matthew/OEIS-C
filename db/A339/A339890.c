/*
 * A339890  Number of odd-length factorizations of n into factors > 1.
 * https://oeis.org/A339890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into factors > 1
 *            with odd length
 * Range:     u8 n <= 8639, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9660 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void count_odd_even(idx_t n, idx_t min_factor, seq_t *odd, seq_t *even)
{
    seq_t o = 0;
    seq_t e = 0;
    for (idx_t d = min_factor; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t sub_o = 0;
            seq_t sub_e = 0;
            count_odd_even(n / d, d, &sub_o, &sub_e);
            o += sub_e;
            e += sub_o;
        }
    }
    /* single‑factor factorisation */
    if (min_factor <= n) {
        o += 1;
    }
    *odd = o;
    *even = e;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t odd = 0;
    seq_t even = 0;
    count_odd_even(n, 2, &odd, &even);
    return odd;
}
