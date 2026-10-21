/*
 * A275361  An eventually quasilinear solution to Hofstadter's Q-recurrence.
 * https://oeis.org/A275361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = 0 a(2) = 4 a(14) = 8 a(15) = 8 For all other n≥1: let m =
 *            ⌊n/8⌋, r = n mod 8 a(n) = 1 if r == 0 5 if r == 1 13 if r == 2
 *            16*m - 40 if r == 3 8*m - 9 if r == 4 8 if r == 5 8*m - 8 if r
 *            == 6 8*m + 7 if r == 7 This agrees with the Hofstadter
 *            Q‑recurrence and its recurrence a(n)=2*a(n‑8)-a(n‑1...
 * Range:     i8 n <= 90, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* small explicit overrides for indices that differ from the generic
       residue formulas */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;
    if (n == 14) return (seq_t)8;
    if (n == 15) return (seq_t)8;

    /* type safe arithmetic: compute floor(n/8) in the natural integer type,
       then cast to seq_t for the final computation */
    seq_t m = (seq_t)(n >> 3);          /* m = ⌊n/8⌋ */
    int r = (int)(n & 7);                /* r = n mod 8 */

    switch (r)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)5;
        case 2: return (seq_t)13;
        case 3: return 16 * m - (seq_t)40;   /* 16*m - 40 */
        case 4: return 8 * m - (seq_t)9;    /* 8*m - 9 */
        case 5: return (seq_t)8;
        case 6: return 8 * m - (seq_t)8;    /* 8*m - 8 */
        case 7: return 8 * m + (seq_t)7;    /* 8*m + 7 */
    }
    return (seq_t)0;  /* technically unreachable */
}
