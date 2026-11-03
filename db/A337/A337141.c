/*
 * A337141  Numbers having at least one 6 in their representation in base 7.
 * https://oeis.org/A337141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9886 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th positive integer that contains the digit 6 in its
   base‑7 representation.
   a(1) = 6, a(2) = 13, …  */
/* The algorithm scans consecutive integers and stops when the n-th
   qualifying integer is found.  This is O(n·digits) which is
   well below the 2 second limit for n ≤ 10 000. */

#include <stdbool.h>

/* Return true if x, written in base 7, contains at least one digit 6. */
static bool has6(seq_t x)
{
    if (x == 0) return false;          /* 0 has no digits 6 */
    while (x > 0) {
        if (x % 7 == 6) return true;   /* found a 6 */
        x /= 7;
    }
    return false;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, behaviour irrelevant */

    seq_t candidate = 1;
    idx_t count = 0;

    while (1) {
        if (has6(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
