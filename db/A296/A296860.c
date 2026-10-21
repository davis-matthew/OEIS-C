/*
 * A296860  Numbers k whose base-2 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th positive integer whose binary representation has
   less pits than peaks.  The function scans successive integers,
   testing the pit/peak condition for each one.  */
/* The pit/peak test is performed by extracting the binary digits
   into a small buffer and counting interior positions.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

static bool in_sequence(seq_t x)
{
    size_t bits = bitlen_seq_t(x);
    if (bits < 3)                     /* need at least one interior digit */
        return false;

    /* Allocate a temporary buffer large enough to hold the bits.  */
    static uint8_t *bits_buf = NULL;
    static size_t buf_cap = 0;
    if (bits > buf_cap) {
        free(bits_buf);
        bits_buf = (uint8_t *)malloc(bits);
        buf_cap = bits;
    }

    /* Extract bits least‑significant first.  */
    seq_t tmp = x;
    for (size_t i = 0; i < bits; ++i) {
        bits_buf[i] = (uint8_t)(tmp & 1);
        tmp >>= 1;
    }

    unsigned pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < bits; ++i) {
        if (bits_buf[i] == 0 && bits_buf[i - 1] == 1 && bits_buf[i + 1] == 1)
            ++pits;                   /* pit: 101 -> 0 ⟂ 1 1 */
        else if (bits_buf[i] == 1 && bits_buf[i - 1] == 0 && bits_buf[i + 1] == 0)
            ++peaks;                  /* peak: 010 -> 1 ⟂ 0 0 */
    }

    return pits < peaks;
}

/* Return the n-th element (n >= 1) of A296860. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1, return 0 */

    idx_t   found = 0;
    seq_t   k = 0;
    seq_t   result = 0;

    while (found < n) {
        ++k;                           /* test next integer             */
        if (in_sequence(k)) {
            ++found;
            result = k;
        }
    }
    return result;
}
