/*
 * A329326  Length of the co-Lyndon factorization of the reversed binary
 *          expansion of n.
 * https://oeis.org/A329326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the co‑Lyndon factorization of the reversed
   binary expansion of n.  The code implements a variant of Duval’s
   algorithm with the alphabet reversed (1 < 0). */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract the bits of n; bits[0] is the least significant bit,
       which is the first bit of the reversed binary expansion. */
    unsigned bits[64];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        bits[len++] = (unsigned)(tmp & 1);
        tmp >>= 1;
    }
    if (len == 0) {          /* n == 0, not used for the offset 1 */
        return 0;
    }

    /* Duval algorithm adapted for the reversed alphabet.
       In the usual Duval algorithm we test s[k] <= s[j];
       after inverting the alphabet we can use the equivalent test
       bits[j] <= bits[k]. */
    size_t i = 0;
    seq_t count = 0;
    while (i < len) {
        size_t j = i + 1;
        size_t k = i;
        while (j < len && bits[j] <= bits[k]) {
            if (bits[j] < bits[k]) {
                k = i;
            } else {
                ++k;
            }
            ++j;
        }
        size_t l = j - k;
        while (i <= k) {
            ++count;
            i += l;
        }
    }
    return count;
}
