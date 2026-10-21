/*
 * A296901  Numbers whose base-16 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer whose base‑16 digits have
   more pits than peaks (pit = digit i with both neighbors larger,
   peak = digit i with both neighbors smaller).  The term is
   found by scanning the natural numbers starting at 256. */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a number has more pits than peaks in base 16. */
static bool more_pits_than_peaks(uint64_t x)
{
    /* Gather base‑16 digits (least significant first). */
    uint8_t digits[64];
    int len = 0;
    while (x) {
        digits[len++] = (uint8_t)(x & 0xF);   /* x % 16 */
        x >>= 4;                             /* x /= 16 */
    }

    /* A number with fewer than 3 digits can never have a pit or peak. */
    if (len < 3)
        return false;

    int pits  = 0;
    int peaks = 0;
    for (int i = 1; i + 1 < len; ++i) {
        uint8_t prev = digits[i - 1];
        uint8_t cur  = digits[i];
        uint8_t next = digits[i + 1];
        if (prev > cur && next > cur)
            ++pits;
        else if (prev < cur && next < cur)
            ++peaks;
    }
    return pits > peaks;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t candidate = 256;        /* first number with ≥3 base‑16 digits */
    idx_t found = 0;

    while (found < n) {
        if (more_pits_than_peaks((uint64_t)candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
    return 0; /* should never reach here */
}
