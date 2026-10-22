/*
 * A343335  a(n) is the smallest proper alternating multiple of n when n is
 *          not a multiple of 20, otherwise a(20*k) = 0 for k >= 1.
 * https://oeis.org/A343335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A343335:
   a(n) = smallest proper alternating multiple of n (k > 1),
   returning 0 when n is a multiple of 20. */
#include <stdbool.h>

/* Test whether a decimal number has alternating parity of consecutive digits. */
static bool is_alternating(seq_t x)
{
    if (x < 10) return true;             /* single‑digit numbers are alternating */
    int last = (int)(x % 10) & 1;        /* parity of the last digit (units) */
    x /= 10;
    while (x > 0) {
        int p = (int)(x % 10) & 1;        /* parity of the current digit */
        if (p == last) return false;      /* two adjacent digits share parity */
        last = p;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Multiples of 20 are not alternators. */
    if (n % 20ULL == 0ULL) return 0;

    for (idx_t k = 2;; ++k) {
        seq_t m = (seq_t)n * k;
        if (is_alternating(m)) return m;
    }
}
