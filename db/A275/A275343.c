/*
 * A275343  Numbers with at least three digits and with the property that the
 *          sum of the cubes of the first and last digit equals the number
 *          obtained when the first and last digits are deleted.
 * https://oeis.org/A275343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth smallest number >= 100 such that * (first digit)^3 +
 *            (last digit)^3 equals the integer formed * by deleting the first
 *            and last digits. The sequence is obtained * by merging 90
 *            increasing subsequences indexed by the first digit * d=1..9 and
 *            the last digit e=0..9. Each subsequence is * n...
 * Range:     u8 none, u16 none, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

#define SUBCNT 90

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 */

    /*---------------------------------------------------------------------*
     *  Declare static arrays with the constant parts of each subsequence *
     *---------------------------------------------------------------------*/
    static seq_t const_d[SUBCNT], const_e[SUBCNT], const_m[SUBCNT];
    static unsigned const_s[SUBCNT];
    static bool init_done = false;

    if (!init_done) {
        unsigned idx = 0;
        for (int d = 1; d <= 9; ++d) {             /* first digit must be non‑zero */
            for (int e = 0; e <= 9; ++e) {         /* last digit can be zero */
                const_d[idx] = (seq_t)d;
                const_e[idx] = (seq_t)e;
                seq_t m = (seq_t)(d*d*d + e*e*e);   /* m = d^3 + e^3  */
                const_m[idx] = m;
                const_s[idx] = (unsigned)digit_len(m); /* number of digits of m */
                ++idx;
            }
        }
        init_done = true;
    }

    /*---------------------------------------------------------------------*
     *  Local state for the generation of the nth term.  We keep the      *
     *  current power of 10 for each subsequence.  These are independent   *
     *  of any previous call to a() hence the function is order‑independent *
     *---------------------------------------------------------------------*/
    seq_t pow[SUBCNT];
    for (unsigned i = 0; i < SUBCNT; ++i) {
        pow[i] = pow10(const_s[i] + 1);            /* k = 0  -> exponent s+1 */
    }

    /*---------------------------------------------------------------------*
     *  Merge the 90 subsequences until the nth element is reached.       *
     *---------------------------------------------------------------------*/
    idx_t count = 0;
    seq_t best = 0;

    while (count < n) {
        /* Find minimal candidate among the 90 current values */
        unsigned idx_min = 0;
        /* candidate for subsequence 0 */
        best = const_d[0] * pow[0] + const_m[0] * (seq_t)10 + const_e[0];
        for (unsigned i = 1; i < SUBCNT; ++i) {
            seq_t cand = const_d[i] * pow[i] + const_m[i] * (seq_t)10 + const_e[i];
            if (cand < best) {
                best = cand;
                idx_min = i;
            }
        }

        ++count;
        if (count == n)
            return best;

        /* Advance the chosen subsequence: multiply its power by 10 */
        pow[idx_min] = pow[idx_min] * (seq_t)10;
    }

    /* Control should never reach here because the loop returns above. */
    return best;
}
