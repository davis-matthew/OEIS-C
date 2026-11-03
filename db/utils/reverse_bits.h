/* reverse_bits: n with its binary digits reversed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_REVERSE_BITS_H
#define OEISC_UTILS_REVERSE_BITS_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- bit‑reversal helper for arbitrary‑precision seq_t ---------------- */
static seq_t reverse_bits(seq_t x)
{
    seq_t rev = 0;
    seq_t tmp = x;
    size_t m = 0;

    /* Determine the bit length of x */
    while (tmp != (seq_t)0) {
        tmp >>= 1;
        ++m;
    }

    /* Reverse the m bits of x */
    for (size_t i = 0; i < m; ++i) {
        if (x & ((seq_t)1 << i))
            rev |= ((seq_t)1 << (m - 1 - i));
    }
    return rev;
}

#endif
