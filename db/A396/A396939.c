/*
 * A396939  Numbers having isolated ones or isolated double ones in their
 *          binary representations.
 * https://oeis.org/A396939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer whose binary representation
 *            contains an isolated single '1' or an isolated double '11'. The
 *            sequence is enumerated starting at n = 1
 * Range:     u8 n <= 229, u16 n <= 9696, u32 n <= 9696, u64 n <= 9696,
 *            u128 n <= 9696, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Return true if x has an isolated single 1 or an isolated double 1
   in its binary expansion. */
static bool has_isolated(seq_t x)
{
    unsigned len = bitlen_seq_t(x);          /* number of used bits */

    /* Check for isolated single 1 */
    for (unsigned k = 0; k < len; ++k) {
        bool cur  = (((x >> k) & (seq_t)1) != (seq_t)0);
        if (!cur) continue;
        bool prev = (k == 0) ? false :
                    (((x >> (k - 1)) & (seq_t)1) != (seq_t)0);
        bool next = (k + 1 >= len) ? false :
                    (((x >> (k + 1)) & (seq_t)1) != (seq_t)0);
        if (!prev && !next) return true;
    }

    /* Check for isolated double 1 ('11') */
    for (unsigned k = 0; k + 1 < len; ++k) {
        bool pair = (((x >> k) & (seq_t)3) == (seq_t)3);
        if (!pair) continue;
        bool prev = (k == 0) ? false :
                    (((x >> (k - 1)) & (seq_t)1) != (seq_t)0);
        bool next = (k + 2 >= len) ? false :
                    (((x >> (k + 2)) & (seq_t)1) != (seq_t)0);
        if (!prev && !next) return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller. */
    idx_t count = 0;
    idx_t m = 1;

    for (;; ++m) {
        if (has_isolated((seq_t)m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
