/*
 * A298981  Numbers m such that there does not exist an integer k < m for
 *          which the initial decimal digits of k/m are m.
 * https://oeis.org/A298981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298981: numbers m such that no k < m satisfies
 * floor(10^d * k / m) = m, where d = #decimal digits of m.
 * The algorithm checks each candidate m sequentially and
 * stops when the nth valid m is found. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        int d = num_digits_idx(m);            /* number of decimal digits of m   */
        seq_t div = pow10(d);            /* 10^d as seq_t                    */
        seq_t ms   = (seq_t)m;           /* m as seq_t                        */
        seq_t sq   = ms * ms;            /* m^2                               */
        seq_t L    = (sq + div - 1) / div;/* ceil(m^2 / 10^d)                  */
        seq_t tmp  = (ms + 1) * ms - 1;   /* (m+1)*m - 1                       */
        seq_t U    = tmp / div;          /* floor(((m+1)*m - 1)/10^d)          */
        seq_t maxk = ms - 1;
        seq_t upper = U;
        if (upper > maxk) upper = maxk;   /* respect k < m                     */
        bool excluded = (L <= upper);     /* is there a k that works?          */
        if (!excluded) {
            ++count;
            if (count == n) return ms;
        }
    }
}
