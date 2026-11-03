/*
 * A319744  Partial sums of bouncy numbers (A152054).
 * https://oeis.org/A319744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9925 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of bouncy numbers (A152054). a(n) = sum of first n bouncy numbers. */

#include <stdbool.h>

/* Return true if num is a bouncy number (neither non‑decreasing nor non‑increasing). */
static bool is_bouncy(idx_t num)
{
    /* All bouncy numbers have at least 3 digits. */
    if (num < 100) return false;

    /* Extract digits into an array.  Use a small fixed buffer. */
    unsigned char digs[20];
    int len = 0;
    idx_t tmp = num;
    while (tmp) {
        digs[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }

    bool inc = true, dec = true;
    /* Compare digits from most significant to least. */
    for (int i = len - 2; i >= 0; --i) {
        unsigned char high = digs[i + 1];
        unsigned char low  = digs[i];
        if (low > high) dec = false;
        if (low < high) inc = false;
    }
    return !(inc || dec);          /* not increasing and not decreasing */
}

/* Compute the a(n) value.  We scan forward from 100 and accumulate once
   for each bouncy number.  No dynamic allocation is used, so the code
   works for all seq_t types, including arbitrary‑precision integers. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t   count = 0;
    seq_t   sum   = 0;
    idx_t   num   = 100;   /* the first candidate is 100 */

    while (count < n) {
        if (is_bouncy(num)) {
            sum += (seq_t)num;
            ++count;
        }
        ++num;
    }
    return sum;
}
