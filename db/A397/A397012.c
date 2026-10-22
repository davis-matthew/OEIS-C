/*
 * A397012  Numbers that are a sum of three triangular numbers whose indices
 *          are in arithmetic progression; i.e., numbers of the form 3*T(k) +
 *          d^2, where T(k) = k*(k+1)/2 and k, d are integers.
 * https://oeis.org/A397012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A397012: a(n) is the n‑th smallest integer that can be
 * expressed as 3*T(k)+d² for some integers k,d (T(k)=k(k+1)/2).
 * Computation is performed by an exhaustive search: we enumerate
 * integers m in increasing order and test whether m has such a
 * representation. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of terms found so far   */
    idx_t m = 0;              /* current integer being tested   */

    for (;; ++m) {
        /* Check whether m can be written as 3*T(k)+d²   */
        bool found = false;
        idx_t k = 0;
        for (;;) {
            /* Ck = 3 * T(k) = 3 * k*(k+1)/2                      */
            seq_t Ck = ((seq_t)k * ((seq_t)k + 1) / 2) * 3;

            /* Once Ck exceeds m no larger k can work      */
            if (Ck > (seq_t)m)
                break;

            seq_t diff = (seq_t)m - Ck;
            seq_t d = floor_sqrt(diff);
            if (d * d == diff) {
                found = true;
                break;
            }
            ++k;
        }

        if (found) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
