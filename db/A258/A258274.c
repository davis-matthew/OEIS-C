/*
 * A258274  The smallest number of cents which cannot be made with fewer than
 *          n American coins.
 * https://oeis.org/A258274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 662, u32 n <= 10007, u64 n <= 10007,
 *            u128 n <= 10007, big any n
 * Validated: standard, pass, 65 terms through n = 9473 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A258274 – Smallest amount that requires at least n coins.
 *  Formula (when n ≥ 8):  a(n) = 100*n – 706
 *  For 1 ≤ n < 8 the value is found by a short exhaustive search.
 */

#include <stdint.h>

/* Greedy coin change: minimal number of coins for value v.  */
static seq_t min_coins(seq_t v)
{
    seq_t cnt = 0;
    cnt += v / 100;  v %= 100;
    cnt += v / 50;   v %= 50;
    cnt += v / 25;   v %= 25;
    cnt += v / 10;   v %= 10;
    cnt += v / 5;    v %= 5;
    cnt += v;        /* each remaining dollar is one 1¢ coin */
    return cnt;
}

seq_t a(idx_t n)
{
    if (n >= 8)
        return 100 * n - 706;

    seq_t val = 0;
    while (1) {
        ++val;
        if (min_coins(val) >= n)
            return val;
    }
}
