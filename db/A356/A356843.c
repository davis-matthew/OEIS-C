/*
 * A356843  Numbers k such that the k-th composition in standard order covers
 *          an interval of positive integers (gapless) but contains no 1's.
 * https://oeis.org/A356843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356843: Numbers k such that the k-th composition in
   standard order covers an interval of positive integers (gapless)
   but contains no 1's.  Computed by enumerating k and testing the
   necessary conditions. */
#include <stdint.h>
#include <stdbool.h>

/* Return true if the k‑th composition in standard order contains
   no part equal to 1 and its distinct part values form a consecutive
   interval. */
static bool qualifies(idx_t k)
{
    idx_t kcopy   = k;      /* copy to walk through bits */
    idx_t prev    = 0;      /* previous set bit position (1‑based) */
    bool seen[65] = {0};    /* track distinct part values (parts ≤ 64) */
    int  distinct = 0;
    int  minV = 0, maxV = 0;

    while (kcopy) {
        /* lowest set bit */
        uint64_t lsb = kcopy & -kcopy;
        /* position of this bit (1‑based) */
        int idx  = __builtin_ctzll(kcopy) + 1;
        int part = idx - (int)prev;          /* part size */

        if (part == 1)                       /* contains a 1 → reject */
            return false;

        if (!seen[part]) {
            seen[part] = true;
            distinct++;
            if (distinct == 1) {
                minV = maxV = part;
            } else {
                if (part < minV) minV = part;
                if (part > maxV) maxV = part;
            }
        }
        prev   = (idx_t)idx;
        kcopy ^= lsb;                        /* clear lowest bit */
    }

    if (distinct == 0) return false;         /* empty composition */

    return (maxV - minV + 1) == distinct;   /* consecutive distinct parts? */
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k     = 0;

    while (1) {
        ++k;                                 /* start from k = 1 */
        if (qualifies(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
