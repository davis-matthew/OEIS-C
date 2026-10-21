/*
 * A296898  Numbers whose base-15 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑15 digits contain
 *            more pits than peaks
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_DIGITS 1024

static seq_t *seq        = NULL;
static size_t seq_len    = 0;
static size_t seq_cap    = 0;
static seq_t next_cand   = 1;

/* Return true if x has more pits than peaks in base‑15 representation. */
static bool more_pits_than_peaks(seq_t x)
{
    if (x == 0) return false;

    uint8_t digs[MAX_DIGITS];
    size_t len = 0;
    seq_t y = x;

    while (y != 0) {
        digs[len++] = (uint8_t)(y % 15);
        y /= 15;
    }

    if (len < 3) return false;

    unsigned pits  = 0;
    unsigned peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t a = digs[i - 1];
        uint8_t b = digs[i];
        uint8_t c = digs[i + 1];
        if (a > b && b < c) ++pits;
        if (a < b && b > c) ++peaks;
    }
    return pits > peaks;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (seq_len < (size_t)n) {
        if (more_pits_than_peaks(next_cand)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = next_cand;
        }
        ++next_cand;
    }
    return seq[n - 1];
}
