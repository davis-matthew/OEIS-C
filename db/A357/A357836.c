/*
 * A357836  a(n) = Sum_{k=0..floor((n-2)/3)} Stirling1(n,3*k+2).
 * https://oeis.org/A357836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-2)/3)} Stirling1(n,3*k+2)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exact computation via recurrence on sub‑class sums of signed Stirling numbers of the first kind. */
seq_t a(idx_t n)
{
    seq_t a0 = 1;   /* A_0(0) */
    seq_t a1 = 0;   /* A_1(0) */
    seq_t a2 = 0;   /* A_2(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t mult = (seq_t)(i - 1);      /* (n-1) as seq_t for multiplication */

        seq_t t0 = a2 - mult * a0;        /* A_0(i) = A_2(i-1) - (i-1)*A_0(i-1) */
        seq_t t1 = a0 - mult * a1;        /* A_1(i) = A_0(i-1) - (i-1)*A_1(i-1) */
        seq_t t2 = a1 - mult * a2;        /* A_2(i) = A_1(i-1) - (i-1)*A_2(i-1) */

        a0 = t0;
        a1 = t1;
        a2 = t2;
    }

    return a2;   /* a(n) = A_2(n) */
}
