/* reverse_bits_idx: n with its binary digits reversed.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_REVERSE_BITS_IDX_H
#define OEISC_UTILS_REVERSE_BITS_IDX_H
#include <stdint.h>

/* Reverse the bits of n over the bits up to its most significant 1.
 * For example rev_bits(11) -> 13, since 11 = 1011₂ and 1101₂ = 13. */
static idx_t reverse_bits_idx(idx_t n)
{
    idx_t rev = 0;
    /* Determine number of significant bits */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }
    /* Build the reversed number */
    for (idx_t i = 0; i < len; ++i) {
        rev <<= 1;
        rev |= (n >> i) & 1ULL;
    }
    return rev;
}

#endif
