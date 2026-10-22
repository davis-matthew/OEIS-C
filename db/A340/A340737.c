/*
 * A340737  Numerators of a sequence of fractions converging to e.
 * https://oeis.org/A340737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A340737: Numerators of a sequence of fractions converging to e
   Formula (recurrence):
   a(1) = 3, a(2) = 5, a(3) = 19.
   For n > 3:
     if n is even,
       a(n) = ((n+2)*a(n-1)/2) - a(n-2) - ((n-2)*a(n-3)/2);
     else,
       a(n) = 2*a(n-1) + n*a(n-2);
*/
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)19;

    /* keep the last three computed values */
    seq_t p3 = (seq_t)3;   /* a(n-3) */
    seq_t p2 = (seq_t)5;   /* a(n-2) */
    seq_t p1 = (seq_t)19;  /* a(n-1) */

    seq_t an;
    for (idx_t i = 4; i <= n; ++i) {
        if ((i & 1) == 0) { /* even */
            an = ((seq_t)(i + 2) * p1) / 2
                 - p2
                 - ((seq_t)(i - 2) * p3) / 2;
        } else {            /* odd */
            an = (seq_t)2 * p1 + (seq_t)i * p2;
        }
        /* shift the window */
        p3 = p2;
        p2 = p1;
        p1 = an;
    }
    return an;
}
