/*
 * A376450  n in base whose greedy place values are quarter-squares (A002620).
 * https://oeis.org/A376450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A376450   n in quarter‑square base.
    For each n≥0, compute its greedy expansion in the quarter‑square
    base (A002620 = ⌊k²/4⌋, k≥2).  The expansion uses only 0/1 digits:
    the *i*th digit (from the least significant) is 1 iff the
    i‑th quarter‑square (in ascending order) is used.  The sequence
    a(n) is the decimal integer whose digits are that binary
    representation (e.g. digits 1,0,1 ⇒ 101).  a(0)=0.     */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* ------------------------------------------------------------------
     *  First pass: determine how many quarter‑squares are ≤ n.
     * ------------------------------------------------------------------ */
    idx_t count = 0;
    idx_t k = 2;
    for (;; ++k) {
        /* q = ⌊k²/4⌋  using 128‑bit intermediate to avoid overflow   */
        idx_t q = (unsigned __int128)k * k / 4;
        if (q > n) break;
        ++count;
    }

    /* Allocate space for the quarter‑squares and the digit flags.      */
    /* size_t for malloc size is fine because count is small for all    */
    /* reasonable values of n.                                            */
    idx_t *qs = (idx_t *)malloc((size_t)count * sizeof(idx_t));
    unsigned char *digits = (unsigned char *)malloc((size_t)count);
    if (!qs || !digits) {
        /* Allocation failed – fall back to returning 0.   Must not crash. */
        free(qs);
        free(digits);
        return 0;
    }

    /* ------------------------------------------------------------------
     *  Second pass: fill the quarter‑squares array.
     * ------------------------------------------------------------------ */
    k = 2;
    for (idx_t i = 0; i < count; ++i) {
        qs[i] = (unsigned __int128)k * k / 4;
        ++k;
    }

    /* ------------------------------------------------------------------
     *  Greedy decomposition of n using the quarter‑squares from largest
     *  to smallest.  digits[i] corresponds to qs[i].
     * ------------------------------------------------------------------ */
    idx_t rem = n;
    for (idx_t i = count; i-- > 0; ) {
        if (qs[i] <= rem) {
            digits[i] = 1;
            rem -= qs[i];
        } else {
            digits[i] = 0;
        }
    }

    /* ------------------------------------------------------------------
     *  Build the decimal integer from the binary digit string.
     *  The most significant digit is the last element of the arrays.
     * ------------------------------------------------------------------ */
    seq_t result = 0;
    for (idx_t i = count; i-- > 0; ) {
        result = result * 10 + digits[i];
    }

    free(qs);
    free(digits);
    return result;
}
