/*
 * A350776  Nonnegative integers whose balanced ternary expansion has no two
 *          consecutive nonzero digits.
 * https://oeis.org/A350776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates nonnegative integers whose balanced ternary
   expansion has no two consecutive nonzero digits (digits ±1).  It
   searches sequentially from 0 upwards and returns the n‑th such number
   (n is 1‑based: a(1) = 0). */
#include <stdbool.h>

static inline bool _valid_no_consecutive_nonzero(seq_t val)
{
    seq_t t = val;
    int prev_nonzero = 0;        /* 0 if previous digit was zero,
                                   1 or -1 if it was nonzero   */
    /* Handle the special case val == 0 separately: loop never runs
       and value is considered valid. */
    while (t > (seq_t)0) {
        int r = (int)(t % (seq_t)3);   /* current remainder 0,1,2      */
        t /= (seq_t)3;
        if (r == 2) {                 /* map remainder 2 to -1       */
            r = -1;
            t += (seq_t)1;             /* carry over for balanced ternary */
        }
        if (r != 0) {
            if (prev_nonzero != 0) {
                return false;          /* two consecutive nonzeros   */
            }
            prev_nonzero = r;          /* store sign of current nonzero   */
        } else {
            prev_nonzero = 0;           /* zero digit resets flag      */
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined behaviour for n==0
                                            but return 0 just in case      */
    idx_t count = (idx_t)0;
    seq_t cand = (seq_t)0;
    for (;;) {
        if (_valid_no_consecutive_nonzero(cand)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
        ++cand;
    }
}
