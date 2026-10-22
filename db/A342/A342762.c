/*
 * A342762  Fold a square sheet of paper alternately vertically to the left
 *          and horizontally downwards; after each fold, draw a line along
 *          each inward crease; after n folds, the resulting graph has a(n)
 *          connected components.
 * https://oeis.org/A342762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is defined by a(0..4) = 1, a(5) = 3, a(6) = 5, for n >= 7:
 *            let d(n) = a(n) - a(n-1) d(5) = 2, d(6) = 2 if n is even: d(n) =
 *            2*d(n-1) if n is odd : d(n) = 2*d(n-1) + 2^{(n-7)/2} Then a(n) =
 *            a(n-1)+d(n)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases for n = 0 … 4 */
    if (n < 5) return (seq_t)1;

    /* n = 5 and n = 6 */
/* the following cast to seq_t is only to silence possible warnings
   when seq_t is an arbitrary‑precision class that can be constructed
   from an integer literal. */
    if (n == 5) return (seq_t)3;
    if (n == 6) return (seq_t)5;

    /* For n >= 7, iterate the recurrence. */
    seq_t a_val = (seq_t)5;          /* a(6) */
    seq_t d_prev = (seq_t)2;         /* d(6) = a(6) - a(5) */
    seq_t pow_add = (seq_t)1;        /* 2^{(i-7)/2} for the current odd i */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t d;
        if ((i & 1) == 0) {             /* even i: d = 2*d_prev */
            d = d_prev + d_prev;
        } else {                        /* odd i: d = 2*d_prev + pow_add */
            d = d_prev + d_prev + pow_add;
            pow_add += pow_add;         /* pow_add <<= 1 for next odd i */
        }
        a_val += d;
        d_prev = d;
    }
    return a_val;
}
