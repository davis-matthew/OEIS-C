/*
 * A296884  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296884: Numbers whose base‑10 digits have fewer pits than peaks.
   The function checks each integer, counts the #pits and #peaks in its
   decimal representation, and returns the n‑th number for which pits < peaks.  */

#include <stdint.h>
#include <stdbool.h>

/* Count pits and peaks of the decimal representation of x.  */
static bool pits_less_peaks(uint64_t x)
{
    if (x < 100)        /* less than 3 digits: no interior digits → pits == peaks == 0 */
        return false;

    uint8_t dig[20];    /* decimal digits (least significant first) */
    int len = 0;
    uint64_t t = x;
    while (t > 0) {
        dig[len++] = t % 10;
        t /= 10;
    }

    int pits  = 0;
    int peaks = 0;
    for (int i = 1; i + 1 < len; ++i) {
        if (dig[i-1] > dig[i]  &&  dig[i] < dig[i+1])
            ++pits;
        else if (dig[i-1] < dig[i]  &&  dig[i] > dig[i+1])
            ++peaks;
    }
    return pits < peaks;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Iterate through natural numbers until we find the nth qualifying one. */
    idx_t   desired = n;
    idx_t   encountered = 0;
    uint64_t x = 0;        /* integer to test */

    while (true) {
        if (pits_less_peaks(x)) {
            ++encountered;
            if (encountered == desired)
                return (seq_t)x;
        }
        ++x;
    }
}
