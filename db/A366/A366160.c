/*
 * A366160  Numbers whose binary expansion is not quasiperiodic.
 * https://oeis.org/A366160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose binary expansion is not
 *            quasiperiodic (no proper binary substring covers all positions)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force test each integer in order, skipping those whose binary
   representation admits a proper quasiperiod.  The search is efficient
   because the binary length of the n‑th term for n <= 10000 is < 128
   bits. */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Test whether x has a proper quasiperiod.
   Returns true if some proper binary substring q of x covers the whole
   string of bits of x. */
static bool _is_quasiperiodic(seq_t x)
{
    /* Get bit length and fill an array of bits (msb first). */
    unsigned len = bitlen_seq_t(x);
    if (len <= 1)            /* no proper substring possible */
        return false;

    unsigned char bits[128];
    for (unsigned i = 0; i < len; ++i) {
        bits[i] = (unsigned char)((x >> (len - 1 - i)) & 1U);
    }

    /* Try every possible quasiperiod length m (1 .. len-1). */
    for (unsigned m = 1; m < len; ++m) {
        unsigned max_start = len - m;                 /* last start of a block of length m */

        for (unsigned start = 0; start <= max_start; ++start) {
            /* The candidate substring bits[start .. start+m-1] is the quasiperiod. */

            bool covered[128] = {false};

            /* Scan all occurrences of this candidate in the bit string. */
            for (unsigned pos = 0; pos <= max_start; ++pos) {
                /* Check equality of bits[pos .. pos+m-1] with the candidate. */
                unsigned k;
                for (k = 0; k < m; ++k) {
                    if (bits[pos + k] != bits[start + k]) break;
                }
                if (k == m) { /* found an occurrence */
                    for (k = pos; k < pos + m; ++k) covered[k] = true;
                }
            }

            /* Verify that all positions are covered. */
            bool all_covered = true;
            for (unsigned k = 0; k < len; ++k) {
                if (!covered[k]) { all_covered = false; break; }
            }
            if (all_covered) return true;  /* proper quasiperiod found */
        }
    }
    return false;   /* no proper quasiperiod exists */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* should not be called with n=0 */

    idx_t found = 0;
    seq_t x = 1;

    for (;;) {
        if (!_is_quasiperiodic(x)) {
            ++found;
            if (found == n) return x;
        }
        ++x;
    }
}
