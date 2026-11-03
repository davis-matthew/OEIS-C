/*
 * A348520  Pentaphobe or 5-phobe numbers: integers that are not pentaphile
 *          numbers.
 * https://oeis.org/A348520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: full, pass, 68 terms through n = 68 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pentaphobe numbers: integers that cannot be expressed as a sum of five
   strictly increasing positive integers each dividing the next.
   This implementation precomputes all pentaphobe numbers up to 240
   (the known maximum) using a brute‑force search and returns the nth one
   (1‑based).  For n greater than the number of pentaphobes, it returns 0. */

#include <stdbool.h>
#include <stddef.h>

/* Test whether m is a pentaphile number.  Return true if such a
   representation exists, false otherwise.  The loops are written with
   guard conditions that avoid unsigned underflow for small m. */
static bool isPentaphile (seq_t m)
{
    /* Convert to idx_t for loop bounds (m fits into idx_t). */
    idx_t mm = (idx_t)m;

    for (idx_t b1 = 1; b1 + 4 <= mm; ++b1) {
        for (idx_t b2 = 2 * b1; b2 + 3 <= mm; b2 += b1) {
            for (idx_t b3 = 2 * b2; b3 + 2 <= mm; b3 += b2) {
                for (idx_t b4 = 2 * b3; b4 + 1 <= mm; b4 += b3) {
                    seq_t sum4 = (seq_t)b1 + (seq_t)b2 + (seq_t)b3 + (seq_t)b4;
                    if (sum4 >= m) break;           /* further b4 will only be larger */
                    seq_t b5 = m - sum4;
                    if (b5 > (seq_t)b4 && (b5 % (seq_t)b4) == 0)
                        return true;                 /* representation found */
                }
            }
        }
    }
    return false;                                   /* no representation found */
}

/* Return the nth pentaphobe number (1‑based).  If n is out of range,
   return 0.  The array is built on first use. */
seq_t a (idx_t n)
{
    /* static cache of pentaphobe numbers */
    static seq_t pent[200];
    static size_t pent_len = 0;
    static bool done = false;

    if (!done) {
        /* Precompute all pentaphobe numbers up to the known maximum. */
        const idx_t LIMIT = 300;                      /* > 240, safe upper bound */
        for (idx_t m = 1; m <= LIMIT; ++m) {
            if (!isPentaphile ((seq_t)m))
                pent[pent_len++] = (seq_t)m;
        }
        done = true;
    }

    if (n == 0 || n > pent_len)
        return 0;               /* undefined for out‑of‑range indices */

    return pent[(size_t)(n - 1)];
}
