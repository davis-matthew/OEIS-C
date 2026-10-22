/*
 * A352729  The binary expansion of a(n) contains the runs of consecutive 1's
 *          that appear both in the binary expansions of n and n+1.
 * https://oeis.org/A352729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A352727(n, n+1). A(n) is the integer whose binary
 *            representation consists of the runs of consecutive 1's that
 *            appear in both the binary expansion of n and the binary
 *            expansion of n+1
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type */
    seq_t N = (seq_t)n;   /* binary of n   */
    seq_t M = N + 1;      /* binary of n+1 */

    seq_t result = 0;

    /* number of bits needed to represent N */
    unsigned limit = bitlen_seq_t(N);

    idx_t i = 0;
    while (i < limit) {
        /* Skip zeros */
        if (!((N >> i) & 1)) {
            ++i;
            continue;
        }

        /* Found the start of a run in N */
        idx_t start = i;
        idx_t len   = 0;
        while ( ((N >> (start + len)) & 1) && (start + len < limit) ) {
            ++len;
        }

        /* Check that M has the same run */
        bool same = true;

        /* Run bits must all be 1 in M */
        for (idx_t k = 0; k < len; ++k) {
            if ( !((M >> (start + k)) & 1) ) {
                same = false;
                break;
            }
        }

        /* The run must be maximal in M as well */
        if (same) {
            if (start > 0 && ((M >> (start - 1)) & 1)) {
                same = false; /* M has a longer run to the left */
            }
            if ( ((M >> (start + len)) & 1) ) {
                same = false; /* M has a longer run to the right */
            }
        }

        if (same) {
            /* Add the run to the result bitwise */
            for (idx_t k = 0; k < len; ++k) {
                result |= ((seq_t)1 << (start + k));
            }
        }

        /* Advance past this run */
        i = start + len;
    }

    return result;
}
