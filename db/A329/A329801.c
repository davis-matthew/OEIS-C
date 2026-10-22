/*
 * A329801  Expansion of Sum_{k>=1} x^(k*(k + 1)/2) / (1 + x^(k*(k + 1)/2)).
 * https://oeis.org/A329801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k>=1, T_k = k(k+1)/2 | n} (-1)^(n/T_k + 1)
 * Range:     i8 n <= 9652, i16 n <= 9652, i32 n <= 9652, i64 n <= 9652,
 *            i128 n <= 9652, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where T_k are triangular numbers. */
seq_t a(idx_t n)
{
    seq_t sum   = 0;            /* running total of the sequence value  */
    idx_t n_idx = n;            /* keep the index in its native type   */
    for (idx_t k = 1; ; ++k)
    {
        /* Triangular number T_k = k(k+1)/2, computed in the native type */
        idx_t T = (k * (k + 1)) / 2;
        if (T > n_idx) break;                 /* further T will be too big   */
        if (n_idx % T == 0)                   /* T divides n                 */
        {
            idx_t q = n_idx / T;              /* quotient n/T               */
            if ((q & 1) != 0)                 /* n/T is odd → sign +1      */
                sum += 1;
            else                               /* n/T even  → sign –1      */
                sum -= 1;
        }
    }
    return sum;
}
