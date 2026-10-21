/*
 * A296879  Numbers whose base-9 digits d(m), d(m-1), ..., d(0) have #(pits) =
 *          #(peaks); see Comments.
 * https://oeis.org/A296879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A296879: numbers whose base‑9 digits have equal numbers of pits and peaks.
   a(1) = 1, a(2) = 2,  …  The function returns the nth term of this sequence. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for generated terms */
    static seq_t *seq      = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;
    static idx_t  next_candidate = 1;   /* next natural number to test */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        /* check next_candidate for equal peaks and pits in base‑9 representation */
        idx_t temp = next_candidate;
        unsigned char digits[25];
        size_t dlen = 0;
        while (temp > 0) {
            digits[dlen++] = (unsigned char)(temp % 9);
            temp /= 9;
        }
        if (dlen == 0) dlen = 1;   /* not used for n≥1 but keeps code tidy */

        int pits = 0;
        int peaks = 0;
        if (dlen >= 3) {
            for (size_t i = 1; i + 1 < dlen; ++i) {
                unsigned char dl = digits[i - 1];
                unsigned char d  = digits[i];
                unsigned char dr = digits[i + 1];
                if (dl > d && d < dr) ++pits;
                else if (dl < d && d > dr) ++peaks;
            }
        }

        if (pits == peaks) {
            /* append the term to the cache */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 128;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return seq[n - 1];
}
