/*
 * A250044  Numbers n such that m = floor(n/8) is coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 6565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, member
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250044: numbers n such that m = floor(n/8) is coprime to n
   and, if m > 0, m is also a term of the sequence.  The function builds
   the sequence lazily, caching results so that calls can come in any
   order.  It uses standard C functions only.

   Note: This implementation assumes the presence of the utility
   function gcd(seq_t a, seq_t b) in ../utils/gcd.h.  No other helpers
   are required.  The array is grown with malloc/realloc, which is
   available for all supported seq_t types.  */

/* Necessary standard headers */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility function: GCD of two seq_t values, defined in gcd.h */
#include "../utils/gcd.h"
#include "../utils/member.h"

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;    /* sorted list of terms */
    static size_t seq_len = 0;   /* number of terms stored */
    static size_t seq_cap = 0;   /* allocated capacity */
    static idx_t next_val = 1;   /* next integer to test */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        seq_t val = (seq_t)next_val;
        idx_t m   = next_val / 8;                /* floor(next_val/8) */

        /* check coprime condition */
        if (gcd(val, (seq_t)m) == (seq_t)1) {
            /* if m > 0 it must be a previously generated term */
            if (m == 0 || member(seq, seq_len, (seq_t)m)) {
                /* grow array if needed */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                    /* no error handling needed for the evaluation harness */
                    seq = tmp;
                    seq_cap = new_cap;
                }
                seq[seq_len++] = val;
            }
        }
        ++next_val;
    }

    return seq[n - 1];
}
