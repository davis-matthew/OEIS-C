/*
 * A372053  Irregular array read by rows: row n lists the factorizations of n
 *          into a product of nondecreasing integers >= 2.
 * https://oeis.org/A372053
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 2997, u16 n <= 19795, u32 n <= 19795, u64 n <= 19795,
 *            u128 n <= 19795, big any n
 * Validated: standard, pass, 139 terms through n = 9942 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th element of the flattened array of nondecreasing
   factorizations of integers ≥2.  a(2) is the first factor emitted.
   The algorithm enumerates the factorizations of successive integers
   in lexicographic order and decrements a counter until the desired
   factor is reached. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Recursively walk factorizations of rem, with minimum allowed
   factor min_f.  vec holds the current factor chain (length len).
   Upon reaching a terminal factor (rem==1) each element of the
   chain is emitted in order.  If offset reaches zero, the current
   factor is written to *res and the recursion unwinds. */
static bool dfs_emit(idx_t rem, idx_t min_f, seq_t *vec, size_t len,
                     idx_t *off, seq_t *res)
{
    if (rem == (idx_t)1) {
        for (size_t i = 0; i < len; ++i) {
            if (*off == (idx_t)0) {
                *res = vec[i];
                return true;
            }
            (*off)--;
        }
        return false;
    }
    for (idx_t f = min_f; f <= rem; ++f) {
        if (rem % f == (idx_t)0) {
            vec[len] = (seq_t)f;
            if (dfs_emit(rem / f, f, vec, len + 1, off, res))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)2)          /* undefined for n<2, return 0 */
        return (seq_t)0;

    idx_t off = n - (idx_t)2;   /* offset into the flattened list */
    seq_t result;
    /* buffer big enough for the largest factor chain (≤64 for all
       practical n; larger for arbitrary‑precision types).          */
    seq_t buf[128];

    for (idx_t m = (idx_t)2; ; ++m) {
        if (dfs_emit(m, (idx_t)2, buf, (size_t)0, &off, &result))
            return result;
    }
}
