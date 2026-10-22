/*
 * A329303  If the run lengths in binary expansion of n are (r(1), ..., r(w)),
 *          then the run lengths in binary expansion of a(n) are (r(1), r(3),
 *          r(5), ..., r(6), r(4), r(2)).
 * https://oeis.org/A329303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  For each n, break its binary representation into runs of equal
   bits, reorder the run lengths by taking the odd‑indexed runs in order,
   followed by the even‑indexed runs in reverse order, and rebuild the
   binary number from the new run lengths.  The leading bit stays ‘1’
   (except for n = 0). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t) */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* special case for 0 */

    /* Determine the bit length of n. */
    size_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* Extract run lengths (MSB → LSB). */
    size_t runs[64];
    size_t rc = 0;                      /* number of runs */
    idx_t current_bit = (n >> (bitlen - 1)) & 1;  /* first (MSB) bit */
    size_t count = 1;                   /* first run length */

    for (int pos = (int)bitlen - 2; pos >= 0; --pos) {
        idx_t bit = (n >> pos) & 1;
        if (bit == current_bit) {
            ++count;
        } else {
            runs[rc++] = count;
            count = 1;
            current_bit = bit;
        }
    }
    runs[rc++] = count;                 /* last run */

    /* Reorder run lengths: odds (1,3,5,...) then evens (backwards). */
    size_t new_runs[64];
    size_t nr = 0;
    for (size_t i = 0; i < rc; i += 2)        /* odd indices (0‑based)   */
        new_runs[nr++] = runs[i];
    for (size_t i = rc; i-- > 0; )            /* even indices, reversed */
        if (i % 2 == 1)
            new_runs[nr++] = runs[i];

    /* Rebuild the integer from the reordered run lengths. */
    seq_t result = 0;
    int bit_val = 1;                         /* start with leading 1 */
    for (size_t i = 0; i < nr; ++i) {
        seq_t part = pow2((idx_t)new_runs[i]);    /* 2^len */
        seq_t add  = bit_val ? (part - 1) : 0;
        result = result * part + add;
        bit_val = 1 - bit_val;                  /* toggle bit */
    }
    return result;
}
